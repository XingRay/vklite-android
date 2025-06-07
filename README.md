# vklite-android
vklite demo for android

vklite-android

todo list

- [x] 01 simple triangle (vertex buffer / index buffer)


- [x] 02 single color filled triangle (uniform buffer)


- [x] 03 colored triangle (vertex buffer with multiple descriptors)

- [x] 04 texture image (sampler2D)

- [x] 05 model loading (obj)

- [x] 06 msaa (anti-aliasing) (done)

- [ ] 07 skybox (cubemap)

- [ ] 08 normal mapping

- [ ] 09 tillable texture

- [ ] 06 lighting (phong)

- [ ] 07 shadow mapping

- [ ] 08 post processing (bloom)

- [ ] 09 deferred rendering

- [ ] 10 instancing

- [ ] 11 compute shader

- [ ] 12 particles

- [ ] 13 physics



### todo

demo:

```C++
Test01SimpleTriangle::Test01SimpleTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/01_triangle.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/01_triangle.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                });

        mEngine = vklite::AndroidSimpleGraphicEngineBuilder::asDefault(mApp.window)
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
    }

    void Test01SimpleTriangle::init() {
        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        uint32_t indicesSize = indices.size() * sizeof(uint32_t);
        mIndexBuffer = mEngine->indexBufferBuilder()
                .size(indicesSize)
                .buildUnique();
        mIndexBuffer->update(mEngine->getCommandPool(), indices);
        mEngine->indexBuffer(*mIndexBuffer, indices.size());


        uint32_t verticesSize = vertices.size() * sizeof(Vertex);
        mVertexBuffer = mEngine->vertexBufferBuilder()
                .size(verticesSize)
                .buildUnique();
        mVertexBuffer->update(mEngine->getCommandPool(), vertices.data(), verticesSize);
        mEngine->addVertexBuffer(*mVertexBuffer);
    }

    void Test01SimpleTriangle::drawFrame() {
        mEngine->drawIndexed();
    }
  
```



