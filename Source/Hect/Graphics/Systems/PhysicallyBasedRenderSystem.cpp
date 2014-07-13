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
#include "Hect/Logic/Scene.h"

using namespace hect;

PhysicallyBasedRenderSystem::PhysicallyBasedRenderSystem(Scene& scene, Renderer& renderer, AssetCache& assetCache) :
    RenderSystem(scene, renderer), 
    _buffersInitialized(false)
{
    // Set up additive blending
    _additiveLightState.enable(RenderStateFlag_Blend);
    _additiveLightState.disable(RenderStateFlag_DepthTest);

    _compositorShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/Compositor.shader");
    _screenMesh = assetCache.getHandle<Mesh>("Hect/PhysicallyBased/Screen.mesh");
 
    _directionalLightShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/DirectionalLight.shader");
    _directionalLightColorUniform = &_directionalLightShader->uniformWithName("color");
    _directionalLightDirectionUniform = &_directionalLightShader->uniformWithName("direction");
    _directionalLightViewUniform = &_directionalLightShader->uniformWithName("view");
}

void PhysicallyBasedRenderSystem::renderAll(RenderTarget& target)
{
    // Initialize buffers if needed
    if (!_buffersInitialized)
    {
        _initializeBuffers(target.width(), target.height());
    }
        
    // Begin geometry buffer rendering
    renderer().beginFrame();
    renderer().bindTarget(_geometryBuffer);
    renderer().clear();

    RenderSystem::renderAll(_geometryBuffer);

    renderer().endFrame();

    // Begin accumulation buffer rendering
    renderer().beginFrame();
    renderer().bindTarget(_accumulationBuffer);
    renderer().clear();
    renderer().bindState(_additiveLightState);


    renderer().bindTexture(_geometryBuffer.targets()[0], 0);
    renderer().bindTexture(_geometryBuffer.targets()[1], 1);
    renderer().bindTexture(_geometryBuffer.targets()[2], 2);

    renderer().bindMesh(*_screenMesh);

    // Render directional lights
    renderer().bindShader(*_directionalLightShader);
    renderer().setUniform(*_directionalLightViewUniform, (Matrix4)activeCamera()->viewMatrix());
    for (const DirectionalLight& light : scene().components<DirectionalLight>())
    {
        Vector3 color = light.color();
        Vector3 direction = light.direction();
        renderer().setUniform(*_directionalLightColorUniform, color);
        renderer().setUniform(*_directionalLightDirectionUniform, direction);
        renderer().draw();
    }

    renderer().endFrame();

    // Begin compositor rendering
    renderer().beginFrame();
    renderer().bindTarget(target);
    renderer().clear();

    // Bind the compositor
    renderer().bindShader(*_compositorShader);

    // Bind the accumulation buffer
    renderer().bindTexture(_accumulationBuffer.targets()[0], 0);

    // Bind and draw the final image
    renderer().bindMesh(*_screenMesh);
    renderer().draw();

    renderer().endFrame();
}

void PhysicallyBasedRenderSystem::_initializeBuffers(unsigned width, unsigned height)
{
    _buffersInitialized = true;

    PixelType pixelType = PixelType_Half;
    PixelFormat pixelFormat = PixelFormat_Rgba;
    TextureFilter filter = TextureFilter_Nearest;

    Texture::Array targets;

    // Diffuse: R G B (Roughness)
    targets.push_back(Texture("DiffuseBuffer", width, height, pixelType, pixelFormat, filter, filter, false, false));

    // Specular: R G B A
    targets.push_back(Texture("SpecularBuffer", width, height, pixelType, pixelFormat, filter, filter, false, false));

    // Normal: X Y Z Depth
    targets.push_back(Texture("NormalBuffer", width, height, pixelType, pixelFormat, filter, filter, false, false));
    _geometryBuffer = FrameBuffer(targets);

    targets.clear();
    targets.push_back(Texture("AccumulationBuffer", width, height, pixelType, PixelFormat_Rgb, filter, filter, false, false));
    _accumulationBuffer = FrameBuffer(targets);
}