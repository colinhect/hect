///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "PhysicallyBasedRenderSystem.h"

#include "Hect/Graphics/Components/DirectionalLight.h"
#include "Hect/Graphics/Components/LightProbe.h"
#include "Hect/Graphics/Components/SkyBox.h"
#include "Hect/Logic/Scene.h"
#include "Hect/Spacial/Components/Transform.h"

using namespace hect;

PhysicallyBasedRenderSystem::PhysicallyBasedRenderSystem(Scene& scene, Renderer& renderer, AssetCache& assetCache) :
    RenderSystem(scene, renderer), 
    _buffersInitialized(false)
{
    _compositorShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/Compositor.shader");
    _environmentShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/Environment.shader");
    _directionalLightShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/DirectionalLight.shader");

    _skyBoxMaterial = assetCache.getHandle<Material>("Hect/PhysicallyBased/SkyBox.material");

    _screenMesh = assetCache.getHandle<Mesh>("Hect/PhysicallyBased/Screen.mesh");
    _skyBoxMesh = assetCache.getHandle<Mesh>("Hect/PhysicallyBased/SkyBox.mesh");
}

void PhysicallyBasedRenderSystem::renderAll(RenderTarget& target)
{
    // Initialize buffers if needed
    if (!_buffersInitialized)
    {
        _initializeBuffers(target.width(), target.height());
    }

    // Get the camera
    Component<Camera>::Iter camera = activeCamera();
    if (!camera)
    {
        throw Error("No camera in scene");
    }
    camera->setAspectRatio(target.aspectRatio());

    // Geometry buffer rendering
    {
        renderer().beginFrame();
        renderer().bindTarget(_geometryBuffer);
        renderer().clear();

        // Render the sky box if there is one
        auto skyBox = camera->entity().component<SkyBox>();
        if (skyBox)
        {
            // Construct a transform at the camera's position
            Transform transform;
            transform.setPosition(camera->position());
            transform.updateGlobalTransform();

            // Update the sky box material to use this sky box's texture
            AssetHandle<Texture>::Array textures;
            textures.push_back(skyBox->texture());
            for (Technique& technique : _skyBoxMaterial->techniques())
            {
                for (Pass& pass : technique.passes())
                {
                    pass.setTextures(textures);
                }
            }

            // Render the sky box
            renderMesh(*camera, _geometryBuffer, *_skyBoxMaterial, *_skyBoxMesh, transform);
        }

        // Render each entity in hierarchical order
        for (Entity& entity : scene().entities())
        {
            if (!entity.parent())
            {
                render(*camera, target, entity);
            }
        }

        renderer().endFrame();
    }

    // Accumulation buffer rendering
    {
        renderer().beginFrame();
        renderer().bindTarget(_accumulationBuffer);
        renderer().clear();

        RenderState state;
        state.enable(RenderStateFlag_Blend);
        state.disable(RenderStateFlag_DepthTest);
        state.disable(RenderStateFlag_DepthWrite);
        renderer().bindState(state);

        // Get the first light probe
        auto lightProbe = scene().components<LightProbe>().begin();
        if (!lightProbe)
        {
            throw Error("No light probe in scene");
        }

        renderer().bindTexture(_geometryBuffer.targets()[0], 0);
        renderer().bindTexture(_geometryBuffer.targets()[1], 1);
        renderer().bindTexture(_geometryBuffer.targets()[2], 2);
        renderer().bindTexture(_geometryBuffer.targets()[3], 3);
        renderer().bindTexture(*lightProbe->texture(), 4);
        renderer().bindMesh(*_screenMesh);

        Transform identity;

        // Render environment light
        {
            renderer().bindShader(*_environmentShader);
            setBoundUniforms(*_environmentShader, *camera, target, identity);

            renderer().draw();
        }

        // Render directional lights
        {
            renderer().bindShader(*_directionalLightShader);
            setBoundUniforms(*_directionalLightShader, *camera, target, identity);

            // Get the uniforms required for directional lights
            const Uniform& colorUniform = _directionalLightShader->uniformWithName("lightColor");
            const Uniform& directionUniform = _directionalLightShader->uniformWithName("lightDirection");

            // Render each directional light in the scene
            for (const DirectionalLight& light : scene().components<DirectionalLight>())
            {
                renderer().setUniform(colorUniform, light.color());
                renderer().setUniform(directionUniform, light.direction());
                renderer().draw();
            }
        }

        // Render point lights
        {

        }

        renderer().endFrame();
    }

    // Compositor rendering
    {
        renderer().beginFrame();
        renderer().bindTarget(target);
        renderer().clear();

        RenderState state;
        state.disable(RenderStateFlag_DepthTest);
        renderer().bindState(state);

        renderer().bindShader(*_compositorShader);

        renderer().bindTexture(_geometryBuffer.targets()[0], 0);
        renderer().bindTexture(_accumulationBuffer.targets()[0], 1);

        // Bind and draw the composited image
        renderer().bindMesh(*_screenMesh);
        renderer().draw();

        renderer().endFrame();
    }
}

void PhysicallyBasedRenderSystem::_initializeBuffers(unsigned width, unsigned height)
{
    _buffersInitialized = true;

    TextureFilter filter = TextureFilter_Nearest;

    Texture::Array targets;

    // Diffuse: Red Green Blue
    targets.push_back(Texture("DiffuseBuffer", width, height, PixelType_Half, PixelFormat_Rgb, filter, filter, false, false));

    // Material: Roughness Metallic ?
    targets.push_back(Texture("MaterialBuffer", width, height, PixelType_Half, PixelFormat_Rgb, filter, filter, false, false));

    // Position: X Y Z
    targets.push_back(Texture("PositionBuffer", width, height, PixelType_Half, PixelFormat_Rgb, filter, filter, false, false));

    // Normal: X Y Z Depth
    targets.push_back(Texture("NormalBuffer", width, height, PixelType_Half, PixelFormat_Rgba, filter, filter, false, false));
    _geometryBuffer = FrameBuffer(targets);

    targets.clear();
    targets.push_back(Texture("AccumulationBuffer", width, height, PixelType_Half, PixelFormat_Rgba, filter, filter, false, false));
    _accumulationBuffer = FrameBuffer(targets);
}