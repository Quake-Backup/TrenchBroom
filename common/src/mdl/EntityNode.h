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

#pragma once

#include "Macros.h"
#include "mdl/EntityNodeBase.h"
#include "mdl/HitType.h"
#include "mdl/Object.h"

#include "vm/bbox.h"
#include "vm/util.h"

#include <optional>
#include <vector>

namespace tb::mdl
{
class EntityModel;
enum class PitchType;
struct ModelSpecification;
struct EntityPropertyConfig;

class EntityNode : public EntityNodeBase, public Object
{
public:
  static const HitType::Type EntityHitType;
  static const vm::bbox3d DefaultBounds;

private:
  struct CachedBounds
  {
    vm::bbox3d modelBounds;
    vm::bbox3d logicalBounds;
    vm::bbox3d physicalBounds;
  };
  mutable std::optional<CachedBounds> m_cachedBounds;

public:
  explicit EntityNode(Entity entity);

public: // entity model
  const vm::bbox3d& modelBounds() const;
  void setModel(const EntityModel* model);

private: // implement Node interface
  const vm::bbox3d& doGetLogicalBounds() const override;
  const vm::bbox3d& doGetPhysicalBounds() const override;

  double doGetProjectedArea(vm::axis::type axis) const override;

  Node* doClone(const vm::bbox3d& worldBounds) const override;

  bool doCanAddChild(const Node* child) const override;
  bool doCanRemoveChild(const Node* child) const override;
  bool doRemoveIfEmpty() const override;

  bool doShouldAddToSpacialIndex() const override;

  void doChildWasAdded(Node* node) override;
  void doChildWasRemoved(Node* node) override;

  void doNodePhysicalBoundsDidChange() override;
  void doChildPhysicalBoundsDidChange() override;

  bool doSelectable() const override;

  void doPick(
    const EditorContext& editorContext,
    const vm::ray3d& ray,
    PickResult& pickResult) override;
  void doFindNodesContaining(const vm::vec3d& point, std::vector<Node*>& result) override;

  void doAccept(NodeVisitor& visitor) override;
  void doAccept(ConstNodeVisitor& visitor) const override;

  std::vector<Node*> nodesRequiredForViewSelection() override;

private: // implement EntityNodeBase interface
  void doPropertiesDidChange(const vm::bbox3d& oldBounds) override;
  vm::vec3d doGetLinkSourceAnchor() const override;
  vm::vec3d doGetLinkTargetAnchor() const override;

private: // implement Object interface
  Node* doGetContainer() override;
  LayerNode* doGetContainingLayer() override;
  GroupNode* doGetContainingGroup() override;

private:
  void invalidateBounds();
  void validateBounds() const;

private: // implement Taggable interface
  void doAcceptTagVisitor(TagVisitor& visitor) override;
  void doAcceptTagVisitor(ConstTagVisitor& visitor) const override;

private:
  deleteCopyAndMove(EntityNode);
};

} // namespace tb::mdl
