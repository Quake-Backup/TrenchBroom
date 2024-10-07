/*
 Copyright (C) 2010 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CompilationRunner.h"

#include <QDir>
#include <QMetaEnum>
#include <QProcess>
#include <QtGlobal>

#include "Exceptions.h"
#include "io/DiskIO.h"
#include "io/ExportOptions.h"
#include "io/PathInfo.h"
#include "io/PathMatcher.h"
#include "io/PathQt.h"
#include "io/TraversalMode.h"
#include "mdl/CompilationProfile.h"
#include "mdl/CompilationTask.h"
#include "ui/CompilationContext.h"
#include "ui/CompilationVariables.h"
#include "ui/MapDocument.h" // IWYU pragma: keep

#include "kdl/functional.h"
#include "kdl/overload.h"
#include "kdl/result_fold.h"
#include "kdl/string_utils.h"
#include "kdl/vector_utils.h"

#include <filesystem>
#include <string>

namespace tb::ui
{

CompilationTaskRunner::CompilationTaskRunner(CompilationContext& context)
  : m_context{context}
{
}

CompilationTaskRunner::~CompilationTaskRunner() = default;

void CompilationTaskRunner::execute()
{
  doExecute();
}

void CompilationTaskRunner::terminate()
{
  doTerminate();
}

std::string CompilationTaskRunner::interpolate(const std::string& spec)
{
  try
  {
    return m_context.interpolate(spec);
  }
  catch (const Exception& e)
  {
    m_context << "#### Could not interpolate expression '" << QString::fromStdString(spec)
              << "': " << e.what() << "\n";
    throw;
  }
}

CompilationExportMapTaskRunner::CompilationExportMapTaskRunner(
  CompilationContext& context, mdl::CompilationExportMap task)
  : CompilationTaskRunner{context}
  , m_task{std::move(task)}
{
}

CompilationExportMapTaskRunner::~CompilationExportMapTaskRunner() = default;

void CompilationExportMapTaskRunner::doExecute()
{
  emit start();

  const auto targetPath = std::filesystem::path{interpolate(m_task.targetSpec)};
  m_context << "#### Exporting map file '" << io::pathAsQString(targetPath) << "'\n";

  if (!m_context.test())
  {
    io::Disk::createDirectory(targetPath.parent_path()) | kdl::and_then([&](auto) {
      const auto options = io::MapExportOptions{targetPath};
      const auto document = m_context.document();
      return document->exportDocumentAs(options);
    }) | kdl::transform([&]() { emit end(); })
      | kdl::transform_error([&](auto e) {
          m_context << "#### Could not export map file '" << io::pathAsQString(targetPath)
                    << "': " << QString::fromStdString(e.msg) << "\n";
          emit error();
        });
  }
  else
  {
    emit end();
  }
}

void CompilationExportMapTaskRunner::doTerminate() {}

CompilationCopyFilesTaskRunner::CompilationCopyFilesTaskRunner(
  CompilationContext& context, mdl::CompilationCopyFiles task)
  : CompilationTaskRunner{context}
  , m_task{std::move(task)}
{
}

CompilationCopyFilesTaskRunner::~CompilationCopyFilesTaskRunner() = default;

void CompilationCopyFilesTaskRunner::doExecute()
{
  emit start();

  const auto sourcePath = std::filesystem::path{interpolate(m_task.sourceSpec)};
  const auto targetPath = std::filesystem::path{interpolate(m_task.targetSpec)};

  const auto sourceDirPath = sourcePath.parent_path();
  const auto sourcePathMatcher = kdl::lift_and(
    io::makePathInfoPathMatcher({io::PathInfo::File}),
    io::makeFilenamePathMatcher(sourcePath.filename().string()));

  io::Disk::find(sourceDirPath, io::TraversalMode::Flat, sourcePathMatcher)
    | kdl::and_then([&](const auto& pathsToCopy) {
        const auto pathStrsToCopy = kdl::vec_transform(
          pathsToCopy, [](const auto& path) { return "'" + path.string() + "'"; });

        m_context << "#### Copying to '" << io::pathAsQString(targetPath)
                  << "/': " << QString::fromStdString(kdl::str_join(pathStrsToCopy, ", "))
                  << "\n";
        if (!m_context.test())
        {
          return io::Disk::createDirectory(targetPath) | kdl::and_then([&](auto) {
                   return kdl::vec_transform(
                            pathsToCopy,
                            [&](const auto& pathToCopy) {
                              return io::Disk::copyFile(pathToCopy, targetPath);
                            })
                          | kdl::fold;
                 });
        }
        return Result<void>{};
      })
    | kdl::transform([&]() { emit end(); }) | kdl::transform_error([&](auto e) {
        m_context << "#### Could not copy '" << io::pathAsQString(sourcePath) << "' to '"
                  << io::pathAsQString(targetPath)
                  << "': " << QString::fromStdString(e.msg) << "\n";
        emit error();
      });
}

void CompilationCopyFilesTaskRunner::doTerminate() {}

CompilationRenameFileTaskRunner::CompilationRenameFileTaskRunner(
  CompilationContext& context, mdl::CompilationRenameFile task)
  : CompilationTaskRunner{context}
  , m_task{std::move(task)}
{
}

CompilationRenameFileTaskRunner::~CompilationRenameFileTaskRunner() = default;

void CompilationRenameFileTaskRunner::doExecute()
{
  emit start();

  const auto sourcePath = std::filesystem::path{interpolate(m_task.sourceSpec)};
  const auto targetPath = std::filesystem::path{interpolate(m_task.targetSpec)};

  m_context << "#### Renaming '" << io::pathAsQString(sourcePath) << "' to '"
            << io::pathAsQString(targetPath) << "'\n";
  if (!m_context.test())
  {
    io::Disk::createDirectory(targetPath.parent_path())
      | kdl::and_then([&](auto) { return io::Disk::moveFile(sourcePath, targetPath); })
      | kdl::transform([&]() { emit end(); }) | kdl::transform_error([&](auto e) {
          m_context << "#### Could not rename '" << io::pathAsQString(sourcePath)
                    << "' to '" << io::pathAsQString(targetPath)
                    << "': " << QString::fromStdString(e.msg) << "\n";
          emit error();
        });
  }
  else
  {
    emit end();
  }
}

void CompilationRenameFileTaskRunner::doTerminate() {}

CompilationDeleteFilesTaskRunner::CompilationDeleteFilesTaskRunner(
  CompilationContext& context, mdl::CompilationDeleteFiles task)
  : CompilationTaskRunner{context}
  , m_task{std::move(task)}
{
}

CompilationDeleteFilesTaskRunner::~CompilationDeleteFilesTaskRunner() = default;

void CompilationDeleteFilesTaskRunner::doExecute()
{
  emit start();

  const auto targetPath = std::filesystem::path{interpolate(m_task.targetSpec)};
  const auto targetDirPath = targetPath.parent_path();
  const auto targetPathMatcher = kdl::lift_and(
    io::makePathInfoPathMatcher({io::PathInfo::File}),
    io::makeFilenamePathMatcher(targetPath.filename().string()));

  io::Disk::find(targetDirPath, io::TraversalMode::Recursive, targetPathMatcher)
    | kdl::transform([&](const auto& pathsToDelete) {
        const auto pathStrsToDelete = kdl::vec_transform(
          pathsToDelete, [](const auto& path) { return "'" + path.string() + "'"; });
        m_context << "#### Deleting: "
                  << QString::fromStdString(kdl::str_join(pathStrsToDelete, ", "))
                  << "\n";

        if (!m_context.test())
        {
          return kdl::vec_transform(pathsToDelete, io::Disk::deleteFile) | kdl::fold;
        }
        return Result<std::vector<bool>>{std::vector<bool>{}};
      })
    | kdl::transform([&](auto) { emit end(); }) | kdl::transform_error([&](auto e) {
        m_context << "#### Could not delete '" << io::pathAsQString(targetPath)
                  << "': " << QString::fromStdString(e.msg) << "\n";
        emit error();
      });
}

void CompilationDeleteFilesTaskRunner::doTerminate() {}

CompilationRunToolTaskRunner::CompilationRunToolTaskRunner(
  CompilationContext& context, mdl::CompilationRunTool task)
  : CompilationTaskRunner{context}
  , m_task{std::move(task)}
{
}

CompilationRunToolTaskRunner::~CompilationRunToolTaskRunner() = default;

void CompilationRunToolTaskRunner::doExecute()
{
  startProcess();
}

void CompilationRunToolTaskRunner::doTerminate()
{
  if (m_process)
  {
    disconnect(
      m_process,
      &QProcess::errorOccurred,
      this,
      &CompilationRunToolTaskRunner::processErrorOccurred);
    disconnect(
      m_process,
      QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
      this,
      &CompilationRunToolTaskRunner::processFinished);
    m_process->kill();
    m_context << "\n\n#### Terminated\n";
  }
}

void CompilationRunToolTaskRunner::startProcess()
{
  assert(m_process == nullptr);

  emit start();
  try
  {
    const auto workDir = m_context.variableValue(CompilationVariableNames::WORK_DIR_PATH);
    const auto cmd = this->cmd();

    m_context << "#### Executing '" << QString::fromStdString(cmd) << "'\n";

    if (!m_context.test())
    {
      m_process = new QProcess{this};
      connect(
        m_process,
        &QProcess::errorOccurred,
        this,
        &CompilationRunToolTaskRunner::processErrorOccurred);
      connect(
        m_process,
        QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this,
        &CompilationRunToolTaskRunner::processFinished);
      connect(
        m_process,
        &QProcess::readyReadStandardError,
        this,
        &CompilationRunToolTaskRunner::processReadyReadStandardError);
      connect(
        m_process,
        &QProcess::readyReadStandardOutput,
        this,
        &CompilationRunToolTaskRunner::processReadyReadStandardOutput);

      m_process->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
      m_process->setWorkingDirectory(QString::fromStdString(workDir));
      m_process->start(QString::fromStdString(cmd));
      if (!m_process->waitForStarted())
      {
        emit error();
      }
    }
    else
    {
      emit end();
    }
  }
  catch (const Exception&)
  {
    emit error();
  }
}

std::string CompilationRunToolTaskRunner::cmd()
{
  const auto toolPath = std::filesystem::path{interpolate(m_task.toolSpec)};
  const auto parameters = interpolate(m_task.parameterSpec);
  return toolPath.empty()     ? ""
         : parameters.empty() ? "\"" + toolPath.string() + "\""
                              : "\"" + toolPath.string() + "\" " + parameters;
}

void CompilationRunToolTaskRunner::processErrorOccurred(
  const QProcess::ProcessError processError)
{
  m_context << "#### Error '"
            << QMetaEnum::fromType<QProcess::ProcessError>().valueToKey(processError)
            << "' occurred when communicating with process\n\n";
  emit error();
}

void CompilationRunToolTaskRunner::processFinished(
  const int exitCode, const QProcess::ExitStatus exitStatus)
{
  switch (exitStatus)
  {
  case QProcess::NormalExit:
    m_context << "#### Finished with exit code " << exitCode << "\n\n";
    if (exitCode == 0 || !m_task.treatNonZeroResultCodeAsError)
    {
      emit end();
    }
    else
    {
      emit error();
    }
    break;
  case QProcess::CrashExit:
    m_context << "#### Crashed with exit code " << exitCode << "\n\n";
    emit error();
    break;
  }
}

void CompilationRunToolTaskRunner::processReadyReadStandardError()
{
  if (m_process)
  {
    const QByteArray bytes = m_process->readAllStandardError();
    m_context << QString::fromLocal8Bit(bytes);
  }
}

void CompilationRunToolTaskRunner::processReadyReadStandardOutput()
{
  if (m_process)
  {
    const QByteArray bytes = m_process->readAllStandardOutput();
    m_context << QString::fromLocal8Bit(bytes);
  }
}

CompilationRunner::CompilationRunner(
  CompilationContext context, const mdl::CompilationProfile& profile, QObject* parent)
  : QObject{parent}
  , m_context{std::move(context)}
  , m_taskRunners{createTaskRunners(m_context, profile)}
  , m_currentTask{std::end(m_taskRunners)}
{
}

CompilationRunner::~CompilationRunner() = default;

CompilationRunner::TaskRunnerList CompilationRunner::createTaskRunners(
  CompilationContext& context, const mdl::CompilationProfile& profile)
{
  auto result = TaskRunnerList{};
  for (const auto& task : profile.tasks)
  {
    std::visit(
      kdl::overload(
        [&](const mdl::CompilationExportMap& exportMap) {
          if (exportMap.enabled)
          {
            result.push_back(
              std::make_unique<CompilationExportMapTaskRunner>(context, exportMap));
          }
        },
        [&](const mdl::CompilationCopyFiles& copyFiles) {
          if (copyFiles.enabled)
          {
            result.push_back(
              std::make_unique<CompilationCopyFilesTaskRunner>(context, copyFiles));
          }
        },
        [&](const mdl::CompilationRenameFile& renameFile) {
          if (renameFile.enabled)
          {
            result.push_back(
              std::make_unique<CompilationRenameFileTaskRunner>(context, renameFile));
          }
        },
        [&](const mdl::CompilationDeleteFiles& deleteFiles) {
          if (deleteFiles.enabled)
          {
            result.push_back(
              std::make_unique<CompilationDeleteFilesTaskRunner>(context, deleteFiles));
          }
        },
        [&](const mdl::CompilationRunTool& runTool) {
          if (runTool.enabled)
          {
            result.push_back(
              std::make_unique<CompilationRunToolTaskRunner>(context, runTool));
          }
        }),
      task);
  }
  return result;
}

void CompilationRunner::execute()
{
  assert(!running());

  m_currentTask = std::begin(m_taskRunners);
  if (m_currentTask == std::end(m_taskRunners))
  {
    return;
  }
  bindEvents(*m_currentTask->get());

  emit compilationStarted();

  const auto workDir = QString::fromStdString(
    m_context.variableValue(CompilationVariableNames::WORK_DIR_PATH));
  if (!QDir{workDir}.exists())
  {
    m_context << "#### Error: working directory '" << workDir << "' does not exist\n";
  }
  else
  {
    m_context << "#### Using working directory '" << workDir << "'\n";
  }
  m_currentTask->get()->execute();
}

void CompilationRunner::terminate()
{
  assert(running());
  unbindEvents(*m_currentTask->get());
  m_currentTask->get()->terminate();
  m_currentTask = std::end(m_taskRunners);

  emit compilationEnded();
}

bool CompilationRunner::running() const
{
  return m_currentTask != std::end(m_taskRunners);
}

void CompilationRunner::bindEvents(CompilationTaskRunner& runner) const
{
  connect(&runner, &CompilationTaskRunner::error, this, &CompilationRunner::taskError);
  connect(&runner, &CompilationTaskRunner::end, this, &CompilationRunner::taskEnd);
}

void CompilationRunner::unbindEvents(CompilationTaskRunner& runner) const
{
  runner.disconnect(this);
}

void CompilationRunner::taskError()
{
  if (running())
  {
    unbindEvents(*m_currentTask->get());
    m_currentTask = std::end(m_taskRunners);
    emit compilationEnded();
  }
}

void CompilationRunner::taskEnd()
{
  if (running())
  {
    unbindEvents(*m_currentTask->get());
    ++m_currentTask;
    if (m_currentTask != std::end(m_taskRunners))
    {
      bindEvents(*m_currentTask->get());
      m_currentTask->get()->execute();
    }
    else
    {
      emit compilationEnded();
    }
  }
}

} // namespace tb::ui