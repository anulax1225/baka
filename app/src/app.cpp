#include "bakara/renderer/renderer.h"
#include "bakatools/logging/log.h"
#include "glm/ext/matrix_transform.hpp"
#include <bakara.h>

using namespace Bk;

class TestLayer : public Layer
{
    public:
        TestLayer() : Layer("Test") 
        {
            blueShader = Ref<Shader>(Shader::Create(R"(
                #version 420 core
                layout (location = 0) in vec3 aPos;

                uniform mat4 viewProjection;
                uniform mat4 transform;

                void main() {
                    gl_Position = viewProjection * transform *  vec4(aPos, 1.0);
                }
            )", R"(
                #version 420 core

                out vec4 FragColor;

                void main() {
                    FragColor = vec4(0.0, 0.0, 0.5, 1.0);
                }
            )"));

            float squareVertices[7 * 3]  = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.5f, 0.5f, 0.0f,
                -0.5f, 0.5f, 0.0f, 
            };

            squareVA = Ref<VertexArray>(VertexArray::Create());

            Ref<VertexBuffer> squareVB = Ref<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
            squareVB->SetLayout({
                { ShaderType::Float3, "aPos" }
            });
            squareVA->AddVertexBuffer(squareVB);

            u32 squareIndices[6] = { 0, 1, 2, 0, 2, 3 };
            squareVA->SetIndexbuffer(Ref<IndexBuffer>(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(u32))));
        }

        void OnUpdate(DeltaTime dt) override
        {
            if(Keyboard::KeyDown(KeyCode::W))
                cameraPos.y += cameraSpeed * dt;
            if(Keyboard::KeyDown(KeyCode::S))
               cameraPos.y -= cameraSpeed * dt;
            if(Keyboard::KeyDown(KeyCode::D))
                cameraPos.x += cameraSpeed * dt;
            if(Keyboard::KeyDown(KeyCode::A))
                cameraPos.x -= cameraSpeed * dt;
            if(Keyboard::KeyDown(KeyCode::Q))
                rotation -= rotationSpeed * dt;
            if(Keyboard::KeyDown(KeyCode::E))
                rotation += rotationSpeed * dt;

            camera.SetPosition(cameraPos);
            camera.SetRotation(rotation);

            Renderer::BeginScene(camera);
            RenderCommand::Clear(0.1, 0.1, 0.1, 1.0);
            for(int x = 0; x < 20; x += 2)
                for(int y = 0; y < 20; y += 2)
                    Renderer::Submit(squareVA, blueShader, Math::translate(Mat4(1.0f), Vec3(x, y, 0)));
        }

    private:
        OrthographicCamera camera = { -10.0f, 10.0f, -10.0f, 10.0f };
        Ref<Shader> blueShader;
        Ref<VertexArray> squareVA;
        Vec3 cameraPos = {0, 0, 0};
        float cameraSpeed = 5.0f;
        float rotation = 0.0f;
        float rotationSpeed = 30.0f;
};

class Sandbox : public Application 
{
    public:
        Sandbox() 
        { 
            h_window->SetVsync(true);
            PushLayer(new TestLayer());
        }
};



Application* Bk::CreateApp(int argc, char** argv) {
    return new Sandbox();
}