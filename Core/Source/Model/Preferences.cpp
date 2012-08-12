/*
 Copyright (C) 2010-2012 Kristian Duske
 
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
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Preferences.h"

namespace TrenchBroom {
    namespace Model {
        const std::string Preferences::CameraKey                         = "Controls: camera key";
        const std::string Preferences::CameraOrbitKey                    = "Controls: camera orbit key";
        const std::string Preferences::CameraLookSpeed                   = "Controls: camera look speed";
        const std::string Preferences::CameraPanSpeed                    = "Controls: camera pan speed";
        const std::string Preferences::CameraMoveSpeed                   = "Controls: camera move speed";
        const std::string Preferences::CameraLookInvertY                 = "Controls: invert camera vertical look";
        const std::string Preferences::CameraPanInvertX                  = "Controls: invert camera horizontal pan";
        const std::string Preferences::CameraPanInvertY                  = "Controls: invert camera vertical pan";
        const std::string Preferences::CameraFov                         = "Camera: field of vision";
		const std::string Preferences::SelectionToolMultiKey			 = "Selection: multi selection key";
		const std::string Preferences::SelectionToolGridKey				 = "Grid: modify grid size key";
        const std::string Preferences::ResizeToolKey                     = "Editor: Resize brush key";
        const std::string Preferences::Brightness                        = "Renderer: brightness";
        const std::string Preferences::GridColor                         = "Renderer: grid color";
        const std::string Preferences::FaceColor                         = "Renderer: face color";
        const std::string Preferences::EdgeColor                         = "Renderer: edge color";
        const std::string Preferences::SelectedFaceColor                 = "Renderer: face color (selected)";
        const std::string Preferences::SelectedEdgeColor                 = "Renderer: edge color (selected)";
        const std::string Preferences::HiddenSelectedEdgeColor           = "Renderer: edge color (selected and hidden)";
        const std::string Preferences::EntityBoundsColor                 = "Renderer: entity bounds color";
        const std::string Preferences::EntityBoundsWireframeColor        = "Renderer: entity bounds color (wireframe mode)";
        const std::string Preferences::SelectedEntityBoundsColor         = "Renderer: entity bounds color (selected)";
        const std::string Preferences::HiddenSelectedEntityBoundsColor   = "Renderer: entity bounds color (selected and hidden)";
        const std::string Preferences::SelectionGuideColor               = "Renderer: selection guide color";
        const std::string Preferences::HiddenSelectionGuideColor         = "Renderer: selection guide color (hidden)";
        const std::string Preferences::BackgroundColor                   = "Renderer: background color";
        const std::string Preferences::InfoOverlayColor                  = "Renderer: info overlay color";
        const std::string Preferences::InfoOverlayFadeDistance           = "Renderer: info overlay fade distance";
        const std::string Preferences::SelectedInfoOverlayColor          = "Renderer: info overlay color (selected)";
        const std::string Preferences::SelectedInfoOverlayFadeDistance   = "Renderer: info overlay fade distance (selected)";
        const std::string Preferences::SelectedTextureColor              = "Texture Browser: selected texture color";
        const std::string Preferences::UsedTextureColor                  = "Texture Browser: used texture color";
        const std::string Preferences::OverriddenTextureColor            = "Texture Browser: overridden texture color";
        const std::string Preferences::RendererFontName                  = "Renderer: font name";
        const std::string Preferences::RendererFontSize                  = "Renderer: font size";
        const std::string Preferences::QuakePath                         = "General: quake path";
        const std::string Preferences::VertexHandleSize                  = "General: vertex handle size";
        const std::string Preferences::VertexHandleColor                 = "Renderer: vertex handle color";
        const std::string Preferences::HiddenVertexHandleColor           = "Renderer: vertex handle color (hidden)";
        const std::string Preferences::SelectedVertexHandleColor         = "Renderer: vertex handle color (selected)";
        const std::string Preferences::HiddenSelectedVertexHandleColor   = "Renderer: vertex handle color (selected and hidden)";
        const std::string Preferences::EdgeHandleColor                   = "Renderer: edge handle color";
        const std::string Preferences::HiddenEdgeHandleColor             = "Renderer: edge handle color (hidden)";
        const std::string Preferences::SelectedEdgeHandleColor           = "Renderer: edge handle color (selected)";
        const std::string Preferences::HiddenSelectedEdgeHandleColor     = "Renderer: edge handle color (selected and hidden)";
        const std::string Preferences::FaceHandleColor                   = "Renderer: face handle color";
        const std::string Preferences::HiddenFaceHandleColor             = "Renderer: face handle color (hidden)";
        const std::string Preferences::SelectedFaceHandleColor           = "Renderer: face handle color (selected)";
        const std::string Preferences::HiddenSelectedFaceHandleColor     = "Renderer: face handle color (selected and hidden)";
        
        void Preferences::loadDefaults() {
            m_cameraLookSpeed = 0.5f;
            m_cameraPanSpeed = 0.5f;
            m_cameraMoveSpeed = 0.5f;
            m_cameraLookInvertY = false;
            m_cameraPanInvertX = false;
            m_cameraPanInvertY = false;
            m_cameraFov = 90;
            m_gridColor = Vec4f(1.0f, 1.0f, 1.0f, 0.22f);
            m_faceColor = Vec4f(0.2f, 0.2f, 0.2f, 1);
            m_edgeColor = Vec4f(0.6f, 0.6f, 0.6f, 1.0f);
            m_selectedFaceColor = Vec4f(0.6f, 0.35f, 0.35f, 1);
            m_selectedEdgeColor = Vec4f(1, 0, 0, 1);
            m_hiddenSelectedEdgeColor = Vec4f(1, 0, 0, 0.35f);
            m_entityBoundsColor = Vec4f(0.5f, 0.5f, 0.5f, 1.0f);
            m_entityBoundsWireframeColor = Vec4f(0.5f, 0.5f, 0.5f, 0.6f);
            m_selectedEntityBoundsColor = m_selectedEdgeColor;
            m_hiddenSelectedEntityBoundsColor = m_hiddenSelectedEdgeColor;
            m_selectionGuideColor = m_selectedEdgeColor;
            m_hiddenSelectionGuideColor = m_hiddenSelectedEdgeColor;
            m_backgroundColor = Vec4f(0, 0, 0, 1);
            
            m_infoOverlayColor = Vec4f(1, 1, 1, 1);
            m_infoOverlayFadeDistance = 400;
            m_selectedInfoOverlayColor = Vec4f(1, 0, 0, 1);
            m_selectedInfoOverlayFadeDistance = 2000;
            
            m_selectedTextureColor = Vec4f(0.8f, 0, 0, 1);
            m_usedTextureColor = Vec4f(0.8f, 0.8f, 0, 1);
            m_overriddenTextureColor = Vec4f(0.5f, 0.5f, 0.5f, 1);
            
            m_rendererFontName = "Arial.ttf";
            m_rendererFontSize = 11;
            
			m_brightness = 1.0f;
            
            m_quakePath = "";
            
            m_vertexHandleSize = 1.5f;
            m_vertexHandleColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
            m_hiddenVertexHandleColor = Vec4f(1.0f, 1.0f, 1.0f, 0.35f);
            m_selectedVertexHandleColor = m_selectedEdgeColor;
            m_hiddenSelectedVertexHandleColor = m_hiddenSelectedEdgeColor;
            m_edgeHandleColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
            m_hiddenEdgeHandleColor = Vec4f(1.0f, 1.0f, 1.0f, 0.35f);
            m_selectedEdgeHandleColor = m_selectedEdgeColor;
            m_hiddenSelectedEdgeHandleColor = m_hiddenSelectedEdgeColor;
            m_faceHandleColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
            m_hiddenFaceHandleColor = Vec4f(1.0f, 1.0f, 1.0f, 0.35f);
            m_selectedFaceHandleColor = m_selectedEdgeColor;
            m_hiddenSelectedFaceHandleColor = m_hiddenSelectedEdgeColor;
            
            
            loadPlatformDefaults();
        }
        
        void Preferences::loadPreferences() {
            loadInt(CameraKey, m_cameraKey);
            loadInt(CameraOrbitKey, m_cameraOrbitKey);
            loadFloat(CameraLookSpeed, m_cameraLookSpeed);
            loadFloat(CameraPanSpeed, m_cameraPanSpeed);
            loadFloat(CameraMoveSpeed, m_cameraMoveSpeed);
            loadBool(CameraLookInvertY, m_cameraLookInvertY);
            loadBool(CameraPanInvertX, m_cameraPanInvertX);
            loadBool(CameraPanInvertY, m_cameraPanInvertY);
            loadFloat(CameraFov, m_cameraFov);
			loadInt(SelectionToolMultiKey, m_selectionToolMultiKey);
			loadInt(SelectionToolGridKey, m_selectionToolGridKey);
            loadInt(ResizeToolKey, m_resizeToolKey);
            loadFloat(Brightness, m_brightness);
            loadVec4f(GridColor, m_gridColor);
            loadVec4f(FaceColor, m_faceColor);
            loadVec4f(EdgeColor, m_edgeColor);
            loadVec4f(SelectedFaceColor, m_selectedFaceColor);
            loadVec4f(SelectedEdgeColor, m_selectedEdgeColor);
            loadVec4f(HiddenSelectedEdgeColor, m_hiddenSelectedEdgeColor);
            loadVec4f(EntityBoundsColor, m_entityBoundsColor);
            loadVec4f(EntityBoundsWireframeColor, m_entityBoundsWireframeColor);
            loadVec4f(SelectedEntityBoundsColor, m_selectedEntityBoundsColor);
            loadVec4f(HiddenSelectedEntityBoundsColor, m_hiddenSelectedEntityBoundsColor);
            loadVec4f(SelectionGuideColor, m_selectionGuideColor);
            loadVec4f(HiddenSelectionGuideColor, m_hiddenSelectionGuideColor);
            loadVec4f(BackgroundColor, m_backgroundColor);
			loadVec4f(InfoOverlayColor, m_infoOverlayColor);
			loadFloat(InfoOverlayFadeDistance, m_infoOverlayFadeDistance);
			loadVec4f(SelectedInfoOverlayColor, m_selectedInfoOverlayColor);
			loadFloat(SelectedInfoOverlayFadeDistance, m_selectedInfoOverlayFadeDistance);
            loadVec4f(SelectedTextureColor, m_selectedTextureColor);
            loadVec4f(UsedTextureColor, m_usedTextureColor);
            loadVec4f(OverriddenTextureColor, m_overriddenTextureColor);
			loadString(RendererFontName, m_rendererFontName);
			loadInt(RendererFontSize, m_rendererFontSize);
			loadString(QuakePath, m_quakePath);
            loadFloat(VertexHandleSize, m_vertexHandleSize);
            loadVec4f(VertexHandleColor, m_vertexHandleColor);
            loadVec4f(HiddenVertexHandleColor, m_hiddenVertexHandleColor);
            loadVec4f(SelectedVertexHandleColor, m_selectedVertexHandleColor);
            loadVec4f(HiddenSelectedVertexHandleColor, m_hiddenSelectedVertexHandleColor);
            loadVec4f(EdgeHandleColor, m_edgeHandleColor);
            loadVec4f(HiddenEdgeHandleColor, m_hiddenEdgeHandleColor);
            loadVec4f(SelectedEdgeHandleColor, m_selectedEdgeHandleColor);
            loadVec4f(HiddenSelectedEdgeHandleColor, m_hiddenSelectedEdgeHandleColor);
            loadVec4f(FaceHandleColor, m_faceHandleColor);
            loadVec4f(HiddenFaceHandleColor, m_hiddenFaceHandleColor);
            loadVec4f(SelectedFaceHandleColor, m_selectedFaceHandleColor);
            loadVec4f(HiddenSelectedFaceHandleColor, m_hiddenSelectedFaceHandleColor);
        }

		void Preferences::savePreferences() {
			saveInt(CameraKey, m_cameraKey);
			saveInt(CameraOrbitKey, m_cameraOrbitKey);
            saveFloat(CameraLookSpeed, m_cameraLookSpeed);
            saveFloat(CameraPanSpeed, m_cameraPanSpeed);
            saveFloat(CameraMoveSpeed, m_cameraMoveSpeed);
            saveBool(CameraLookInvertY, m_cameraLookInvertY);
            saveBool(CameraPanInvertX, m_cameraPanInvertX);
            saveBool(CameraPanInvertY, m_cameraPanInvertY);
			saveFloat(CameraFov, m_cameraFov);
			saveInt(SelectionToolMultiKey, m_selectionToolMultiKey);
			saveInt(SelectionToolGridKey, m_selectionToolGridKey);
            saveInt(ResizeToolKey, m_resizeToolKey);
			saveFloat(Brightness, m_brightness);
            saveVec4f(GridColor, m_gridColor);
			saveVec4f(FaceColor, m_faceColor);
			saveVec4f(EdgeColor, m_edgeColor);
			saveVec4f(SelectedFaceColor, m_selectedFaceColor);
			saveVec4f(SelectedEdgeColor, m_selectedEdgeColor);
			saveVec4f(HiddenSelectedEdgeColor, m_hiddenSelectedEdgeColor);
			saveVec4f(EntityBoundsColor, m_entityBoundsColor);
			saveVec4f(EntityBoundsWireframeColor, m_entityBoundsWireframeColor);
			saveVec4f(SelectedEntityBoundsColor, m_selectedEntityBoundsColor);
			saveVec4f(HiddenSelectedEntityBoundsColor, m_hiddenSelectedEntityBoundsColor);
			saveVec4f(SelectionGuideColor, m_selectionGuideColor);
			saveVec4f(HiddenSelectionGuideColor, m_hiddenSelectionGuideColor);
			saveVec4f(BackgroundColor, m_backgroundColor);
			saveVec4f(InfoOverlayColor, m_infoOverlayColor);
			saveFloat(InfoOverlayFadeDistance, m_infoOverlayFadeDistance);
			saveVec4f(SelectedInfoOverlayColor, m_selectedInfoOverlayColor);
			saveFloat(SelectedInfoOverlayFadeDistance, m_selectedInfoOverlayFadeDistance);
            saveVec4f(SelectedTextureColor, m_selectedTextureColor);
            saveVec4f(UsedTextureColor, m_usedTextureColor);
            saveVec4f(OverriddenTextureColor, m_overriddenTextureColor);
			saveString(RendererFontName, m_rendererFontName);
			saveInt(RendererFontSize, m_rendererFontSize);
			saveString(QuakePath, m_quakePath);
            saveFloat(VertexHandleSize, m_vertexHandleSize);
            saveVec4f(VertexHandleColor, m_vertexHandleColor);
            saveVec4f(HiddenVertexHandleColor, m_hiddenVertexHandleColor);
            saveVec4f(SelectedVertexHandleColor, m_selectedVertexHandleColor);
            saveVec4f(HiddenSelectedVertexHandleColor, m_hiddenSelectedVertexHandleColor);
            saveVec4f(EdgeHandleColor, m_edgeHandleColor);
            saveVec4f(HiddenEdgeHandleColor, m_hiddenEdgeHandleColor);
            saveVec4f(SelectedEdgeHandleColor, m_selectedEdgeHandleColor);
            saveVec4f(HiddenSelectedEdgeHandleColor, m_hiddenSelectedEdgeHandleColor);
            saveVec4f(FaceHandleColor, m_faceHandleColor);
            saveVec4f(HiddenFaceHandleColor, m_hiddenFaceHandleColor);
            saveVec4f(SelectedFaceHandleColor, m_selectedFaceHandleColor);
            saveVec4f(HiddenSelectedFaceHandleColor, m_hiddenSelectedFaceHandleColor);
		}

		Preferences* Preferences::sharedPreferences = NULL;

        void Preferences::init() {
            loadDefaults();
            loadPlatformDefaults();
            loadPreferences();
        }
        
		void Preferences::save() {
			savePreferences();
		}

		int Preferences::cameraKey() {
            return m_cameraKey;
        }
        
        int Preferences::cameraOrbitKey() {
            return m_cameraOrbitKey;
        }

        float Preferences::cameraLookSpeed() {
            return m_cameraLookSpeed;
        }
        
        void Preferences::setCameraLookSpeed(float cameraLookSpeed) {
            if (cameraLookSpeed == m_cameraLookSpeed)
                return;
            
            m_cameraLookSpeed = cameraLookSpeed;
            if (saveInstantly())
                saveFloat(CameraLookSpeed, m_cameraLookSpeed);
            preferencesDidChange(CameraLookSpeed);
        }
        
        float Preferences::cameraPanSpeed() {
            return m_cameraPanSpeed;
        }
        
        void Preferences::setCameraPanSpeed(float cameraPanSpeed) {
            if (cameraPanSpeed == m_cameraPanSpeed)
                return;
            
            m_cameraPanSpeed = cameraPanSpeed;
            if (saveInstantly())
                saveFloat(CameraPanSpeed, m_cameraPanSpeed);
            preferencesDidChange(CameraPanSpeed);
        }
        
        float Preferences::cameraMoveSpeed() {
            return m_cameraMoveSpeed;
        }
        
        void Preferences::setCameraMoveSpeed(float cameraMoveSpeed) {
            if (cameraMoveSpeed == m_cameraMoveSpeed)
                return;
            
            m_cameraMoveSpeed = cameraMoveSpeed;
            if (saveInstantly())
                saveFloat(CameraMoveSpeed, m_cameraMoveSpeed);
            preferencesDidChange(CameraMoveSpeed);
        }
        
        bool Preferences::cameraLookInvertY() {
            return m_cameraLookInvertY;
        }
            
        void Preferences::setCameraLookInvertY(bool cameraLookInvertY) {
            if (cameraLookInvertY == m_cameraLookInvertY)
                return;
            
            m_cameraLookInvertY = cameraLookInvertY;
            if (saveInstantly())
                saveBool(CameraLookInvertY, m_cameraLookInvertY);
            preferencesDidChange(CameraLookInvertY);
        }
        
        bool Preferences::cameraPanInvertX() {
            return m_cameraPanInvertX;
        }
        
        void Preferences::setCameraPanInvertX(bool cameraPanInvertX) {
            if (cameraPanInvertX == m_cameraPanInvertX)
                return;
            
            m_cameraPanInvertX = cameraPanInvertX;
            if (saveInstantly())
                saveBool(CameraPanInvertX, m_cameraPanInvertX);
            preferencesDidChange(CameraPanInvertX);
        }
        
        bool Preferences::cameraPanInvertY() {
            return m_cameraPanInvertY;
        }
        
        void Preferences::setCameraPanInvertY(bool cameraPanInvertY) {
            if (cameraPanInvertY == m_cameraPanInvertY)
                return;
            
            m_cameraPanInvertY = cameraPanInvertY;
            if (saveInstantly())
                saveBool(CameraPanInvertY, m_cameraPanInvertY);
            preferencesDidChange(CameraPanInvertY);
        }

        float Preferences::cameraFov() {
            return m_cameraFov;
        }
        
        void Preferences::setCameraFov(float cameraFov) {
            if (cameraFov == m_cameraFov)
                return;
            
            m_cameraFov = cameraFov;
            if (saveInstantly())
                saveFloat(CameraFov, m_cameraFov);
            preferencesDidChange(QuakePath);
        }

        float Preferences::cameraNear() {
            return 10;
        }
        
        float Preferences::cameraFar() {
            return 10000;
        }

		int Preferences::selectionToolMultiKey() {
			return m_selectionToolMultiKey;
		}

		int Preferences::selectionToolGridKey() {
			return m_selectionToolGridKey;
		}

        int Preferences::resizeToolKey() {
            return m_resizeToolKey;
        }
        
        float Preferences::brightness() {
            return m_brightness;
        }

        void Preferences::setBrightness(float brightness) {
            if (brightness == m_brightness)
                return;
            
            m_brightness = brightness;
            if (saveInstantly())
                saveFloat(Brightness, m_brightness);
            preferencesDidChange(QuakePath);
        }
        
        const Vec4f& Preferences::gridColor() {
            return m_gridColor;
        }

        void Preferences::setGridColor(const Vec4f& gridColor) {
            if (gridColor == m_gridColor)
                return;
            
            m_gridColor = gridColor;
            if (saveInstantly())
                saveVec4f(GridColor, m_gridColor);
            preferencesDidChange(GridColor);
        }
        
        const Vec4f& Preferences::faceColor() {
            return m_faceColor;
        }
        
        const Vec4f& Preferences::edgeColor(){
            return m_edgeColor;
        }
        
        const Vec4f& Preferences::selectedFaceColor() {
            return m_selectedFaceColor;
        }
        
        const Vec4f& Preferences::selectedEdgeColor() {
            return m_selectedEdgeColor;
        }
        
        const Vec4f& Preferences::hiddenSelectedEdgeColor() {
            return m_hiddenSelectedEdgeColor;
        }
        
        const Vec4f& Preferences::entityBoundsColor() {
            return m_entityBoundsColor;
        }
        
        const Vec4f& Preferences::entityBoundsWireframeColor() {
            return m_entityBoundsWireframeColor;
        }

        const Vec4f& Preferences::selectedEntityBoundsColor() {
            return m_selectedEntityBoundsColor;
        }
        
        const Vec4f& Preferences::hiddenSelectedEntityBoundsColor() {
            return m_hiddenSelectedEntityBoundsColor;
        }

        const Vec4f& Preferences::selectionGuideColor() {
            return m_selectionGuideColor;
        }
        const Vec4f& Preferences::hiddenSelectionGuideColor() {
            return m_hiddenSelectionGuideColor;
        }

        const Vec4f& Preferences::backgroundColor() {
            return m_backgroundColor;
        }

        const Vec4f& Preferences::infoOverlayColor() {
            return m_infoOverlayColor;
        }
        
        float Preferences::infoOverlayFadeDistance() {
            return m_infoOverlayFadeDistance;
        }
        
        const Vec4f& Preferences::selectedInfoOverlayColor() {
            return m_selectedInfoOverlayColor;
        }
        
        float Preferences::selectedInfoOverlayFadeDistance() {
            return m_selectedInfoOverlayFadeDistance;
        }

        const Vec4f& Preferences::selectedTextureColor() {
            return m_selectedTextureColor;
        }
        
        const Vec4f& Preferences::usedTextureColor() {
            return m_usedTextureColor;
        }
        
        const Vec4f& Preferences::overriddenTextureColor() {
            return m_overriddenTextureColor;
        }

        const std::string& Preferences::rendererFontName() {
            return m_rendererFontName;
        }
        
        unsigned int Preferences::rendererFontSize() {
            return m_rendererFontSize;
        }

        const std::string& Preferences::quakePath() {
            return m_quakePath;
        }

        void Preferences::setQuakePath(const std::string& quakePath) {
            if (quakePath == m_quakePath)
                return;
            
            m_quakePath = quakePath;
            if (saveInstantly())
                saveString(QuakePath, m_quakePath);
            preferencesDidChange(QuakePath);
        }

        float Preferences::resizeHandleSize() {
            return 30.0f;
        }

        float Preferences::vertexHandleSize() {
            return m_vertexHandleSize;
        }

        const Vec4f& Preferences::vertexHandleColor() {
            return m_vertexHandleColor;
        }
        
        const Vec4f& Preferences::hiddenVertexHandleColor() {
            return m_hiddenVertexHandleColor;
        }
        
        const Vec4f& Preferences::selectedVertexHandleColor() {
            return m_selectedVertexHandleColor;
        }
        
        const Vec4f& Preferences::hiddenSelectedVertexHandleColor() {
            return m_hiddenSelectedVertexHandleColor;
        }
        
        const Vec4f& Preferences::edgeHandleColor() {
            return m_edgeHandleColor;
        }
        
        const Vec4f& Preferences::hiddenEdgeHandleColor() {
            return m_hiddenEdgeHandleColor;
        }
        
        const Vec4f& Preferences::selectedEdgeHandleColor() {
            return m_selectedEdgeHandleColor;
        }
        
        const Vec4f& Preferences::hiddenSelectedEdgeHandleColor() {
            return m_hiddenSelectedEdgeHandleColor;
        }
        
        const Vec4f& Preferences::faceHandleColor() {
            return m_faceHandleColor;
        }
        
        const Vec4f& Preferences::hiddenFaceHandleColor() {
            return m_hiddenFaceHandleColor;
        }
        
        const Vec4f& Preferences::selectedFaceHandleColor() {
            return m_selectedFaceHandleColor;
        }
        
        const Vec4f& Preferences::hiddenSelectedFaceHandleColor() {
            return m_hiddenSelectedFaceHandleColor;
        }
    }
}
