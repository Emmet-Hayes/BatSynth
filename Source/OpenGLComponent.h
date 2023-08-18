#pragma once

#include "Utilities.h"
#include "WavefrontObjParser.h"

struct OpenGLUtils
{
    struct Vertex
    {
        float position[3];
        float normal[3];
        float colour[4];
        float texCoord[2];
    };

    struct Attributes
    {
        explicit Attributes (OpenGLShaderProgram& shader)
        {
            position      .reset (createAttribute (shader, "position"));
            normal        .reset (createAttribute (shader, "normal"));
            sourceColour  .reset (createAttribute (shader, "sourceColour"));
            textureCoordIn.reset (createAttribute (shader, "textureCoordIn"));
        }

        void enable()
        {
            using namespace ::juce::gl;

            if (position.get() != nullptr)
            {
                glVertexAttribPointer (position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), nullptr);
                glEnableVertexAttribArray (position->attributeID);
            }

            if (normal.get() != nullptr)
            {
                glVertexAttribPointer (normal->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 3));
                glEnableVertexAttribArray (normal->attributeID);
            }

            if (sourceColour.get() != nullptr)
            {
                glVertexAttribPointer (sourceColour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 6));
                glEnableVertexAttribArray (sourceColour->attributeID);
            }

            if (textureCoordIn.get() != nullptr)
            {
                glVertexAttribPointer (textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 10));
                glEnableVertexAttribArray (textureCoordIn->attributeID);
            }
        }

        void disable()
        {
            using namespace ::juce::gl;

            if (position.get() != nullptr)        glDisableVertexAttribArray (position->attributeID);
            if (normal.get() != nullptr)          glDisableVertexAttribArray (normal->attributeID);
            if (sourceColour.get() != nullptr)    glDisableVertexAttribArray (sourceColour->attributeID);
            if (textureCoordIn.get() != nullptr)  glDisableVertexAttribArray (textureCoordIn->attributeID);
        }

        std::unique_ptr<OpenGLShaderProgram::Attribute> position, normal, sourceColour, textureCoordIn;

    private:
        static OpenGLShaderProgram::Attribute* createAttribute (OpenGLShaderProgram& shader,
                                                                const char* attributeName)
        {
            using namespace ::juce::gl;

            if (glGetAttribLocation (shader.getProgramID(), attributeName) < 0)
                return nullptr;

            return new OpenGLShaderProgram::Attribute (shader, attributeName);
        }
    };

    struct Uniforms
    {
        explicit Uniforms (OpenGLShaderProgram& shader)
        {
            projectionMatrix.reset (createUniform (shader, "projectionMatrix"));
            viewMatrix      .reset (createUniform (shader, "viewMatrix"));
            texture         .reset (createUniform (shader, "demoTexture"));
            lightPosition   .reset (createUniform (shader, "lightPosition"));
            bouncingNumber  .reset (createUniform (shader, "bouncingNumber"));
            viewResolution  .reset (createUniform (shader, "viewResolution"));
            timeKeeper      .reset (createUniform (shader, "timeKeeper"));
            audioAmplitude  .reset (createUniform (shader, "audioAmplitude"));
            synthNoteColor  .reset (createUniform (shader, "synthNoteColor"));
        }

        std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, texture, lightPosition, bouncingNumber, 
                                                      viewResolution, timeKeeper, audioAmplitude, synthNoteColor;

    private:
        static OpenGLShaderProgram::Uniform* createUniform (OpenGLShaderProgram& shader,
                                                            const char* uniformName)
        {
            using namespace ::juce::gl;

            if (glGetUniformLocation (shader.getProgramID(), uniformName) < 0)
                return nullptr;

            return new OpenGLShaderProgram::Uniform (shader, uniformName);
        }
    };

    struct Shape
    {
        Shape()
        {
            juce::String crabString = loadEntireAssetIntoString("teapot.obj");
            if (shapeFile.load (crabString).wasOk())
                for (auto* s : shapeFile.shapes)
                    vertexBuffers.add (new VertexBuffer (*s));
        }

        void draw (Attributes& attributes)
        {
            using namespace ::juce::gl;

            for (auto* vertexBuffer : vertexBuffers)
            {
                vertexBuffer->bind();

                attributes.enable();
                glDrawElements (GL_TRIANGLES, vertexBuffer->numIndices, GL_UNSIGNED_INT, nullptr);
                attributes.disable();
            }
        }

    private:
        struct VertexBuffer
        {
            explicit VertexBuffer (WavefrontObjFile::Shape& shape)
            {
                using namespace ::juce::gl;

                numIndices = shape.mesh.indices.size();

                glGenBuffers (1, &vertexBuffer);
                glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);

                Array<Vertex> vertices;
                createVertexListFromMesh (shape.mesh, vertices, Colours::green);

                glBufferData (GL_ARRAY_BUFFER, vertices.size() * (int) sizeof (Vertex),
                              vertices.getRawDataPointer(), GL_STATIC_DRAW);

                glGenBuffers (1, &indexBuffer);
                glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
                glBufferData (GL_ELEMENT_ARRAY_BUFFER, numIndices * (int) sizeof (juce::uint32),
                                                       shape.mesh.indices.getRawDataPointer(), GL_STATIC_DRAW);
            }

            ~VertexBuffer()
            {
                using namespace ::juce::gl;

                glDeleteBuffers (1, &vertexBuffer);
                glDeleteBuffers (1, &indexBuffer);
            }

            void bind()
            {
                using namespace ::juce::gl;

                glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
                glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            }

            GLuint vertexBuffer, indexBuffer;
            int numIndices;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VertexBuffer)
        };

        WavefrontObjFile shapeFile;
        OwnedArray<VertexBuffer> vertexBuffers;

        static void createVertexListFromMesh (const WavefrontObjFile::Mesh& mesh, Array<Vertex>& list, Colour colour)
        {
            auto scale = 0.2f;
            WavefrontObjFile::TextureCoord defaultTexCoord = { 0.5f, 0.5f };
            WavefrontObjFile::Vertex defaultNormal = { 0.5f, 0.5f, 0.5f };

            for (int i = 0; i < mesh.vertices.size(); ++i)
            {
                auto& v = mesh.vertices.getReference (i);

                auto& n = (i < mesh.normals.size() ? mesh.normals.getReference (i)
                                                   : defaultNormal);

                auto& tc = (i < mesh.textureCoords.size() ? mesh.textureCoords.getReference (i)
                                                          : defaultTexCoord);

                list.add ({ { scale * v.x, scale * v.y, scale * v.z, },
                            { scale * n.x, scale * n.y, scale * n.z, },
                            { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
                            { tc.x, tc.y } });
            }
        }
    };

    struct ShaderPreset
    {
        const char* name;
        const char* vertexShader;
        const char* fragmentShader;
    };

    static Array<ShaderPreset> getPresets()
    {
        #define SHADER_DEMO_HEADER \
            "/*  This is a live OpenGL Shader demo.\n" \
            "    Edit the shader program below and it will be \n" \
            "    compiled and applied to the model above!\n" \
            "*/\n\n"

        ShaderPreset presets[] =
        {
            {
                "Fractal Pyramid",

                SHADER_DEMO_HEADER
                "attribute vec2 position;\n"
                "attribute vec4 normal;\n"
                "attribute vec4 sourceColour;\n"
                "attribute vec2 textureCoordIn;\n"
                "\n"
                "uniform mat4 projectionMatrix;\n"
                "uniform mat4 viewMatrix;\n"
                "uniform vec4 lightPosition;\n"
                "\n"
                "varying vec4 destinationColour;\n"
                "varying vec2 textureCoordOut;\n"
                "varying float lightIntensity;\n"
                "\n"
                "void main()\n"
                "{\n"
                "textureCoordOut = position * 0.5 + 0.5;\n"
                "gl_Position = vec4(position, 0.0, 1.0);\n"
                "}\n",

                SHADER_DEMO_HEADER
               #if JUCE_OPENGL_ES
                "varying lowp vec4 destinationColour;\n"
                "varying lowp vec2 textureCoordOut;\n"
               #else
                "varying vec4 destinationColour;\n"
                "varying vec2 textureCoordOut;\n"
                "uniform float timeKeeper;\n"
                "uniform vec2 viewResolution;\n"
                "uniform float audioAmplitude;\n"
                "uniform float synthNoteColor;\n\n"
               #endif
                "\n"
                "uniform sampler2D demoTexture;\n"
                "\n"
                "vec3 palette(float t)\n"
                "{\n"
                "   vec3 a = vec3(0.6, 0.4, 0.5);\n"
                "   vec3 b = vec3(0.7, 0.3, 0.5);\n"
                "   vec3 c = vec3(0.8, 1.0, 0.6);\n"
                "   vec3 d = vec3(0.76, 0.41, 0.96);\n\n"
                "   return a + b * cos(6.28318 * (c * t + d));\n"
                "}\n"
                "vec2 rotate(vec2 p,float a)\n"
                "{\n"
                "   float c = cos(a);\n"
                "   float s = sin(a);\n"
                "   return p * mat2(c,s,-s,c);\n"
                "}\n"
                "\n"
                "float map(vec3 p)\n"
                "{\n"
                "   for (int i = 0; i < 8; ++i)\n"
                "   {\n"
                "       float t = timeKeeper * 0.2;\n"
                "       p.xz = rotate(p.xz,t);\n"
                "       p.xy = rotate(p.xy,t * 1.89);\n"
                "       p.xz = abs(p.xz);\n"
                "       p.xz -= .5;\n"
                "   }\n"
                "   return dot(sign(p),p) / 5.;\n"
                "}\n"
                "\n"
                "vec4 rm(vec3 ro, vec3 rd)\n"
                "{\n"
                "   float t = 0.;\n"
                "   vec3 col = vec3(0.);\n"
                "   float d;\n"
                "   for (float i = 0.; i < 64.; i++)\n"
                "   {\n"
                "       vec3 p = ro + rd * t;\n"
                "       d = map(p) * .5;\n"
                "       if (d < 0.02)\n"
                "           break;\n"
                "       if (d > 100.)\n"
                "           break;\n"

                "       col += palette(synthNoteColor + timeKeeper * 0.4) / (400. * (d));\n"
                "       t += d;\n"
                "   }\n"
                "   return vec4(col,1. / (d * 100.));\n"
                "}\n"
                "void main()\n"
                "{\n"
                "    vec2 uv = (gl_FragCoord.xy - (viewResolution.xy / 2.)) / viewResolution.x;\n"
                "    vec3 ro = vec3(0.,0.,-50.);\n"
                "    ro.xz = rotate(ro.xz,timeKeeper);\n"
                "    vec3 cf = normalize(-ro);\n"
                "    vec3 cs = normalize(cross(cf,vec3(0.,1.,0.)));\n"
                "    vec3 cu = normalize(cross(cf,cs));\n"
                "    vec3 uuv = ro + cf * 3. + uv.x * cs + uv.y * cu;\n"
                "    vec3 rd = normalize(uuv - ro);\n"
                "    vec4 col = rm(ro,rd) + audioAmplitude;\n"
                "    gl_FragColor = col; \n"
                "}\n"
            },
            {
                "Fractal Kaleidoscope",

                SHADER_DEMO_HEADER
                "attribute vec2 position;\n"
                "attribute vec4 normal;\n"
                "attribute vec4 sourceColour;\n"
                "attribute vec2 textureCoordIn;\n"
                "\n"
                "uniform mat4 projectionMatrix;\n"
                "uniform mat4 viewMatrix;\n"
                "uniform vec4 lightPosition;\n"
                "\n"
                "varying vec4 destinationColour;\n"
                "varying vec2 textureCoordOut;\n"
                "varying float lightIntensity;\n"
                "\n"
                "void main()\n"
                "{\n"
                "textureCoordOut = position * 0.5 + 0.5;\n"
                "gl_Position = vec4(position, 0.0, 1.0);\n"
                "}\n",

                SHADER_DEMO_HEADER
               #if JUCE_OPENGL_ES
                "varying lowp vec4 destinationColour;\n"
                "varying lowp vec2 textureCoordOut;\n"
                "varying highp float lightIntensity;\n"
               #else
                "varying vec4 destinationColour;\n"
                "varying vec2 textureCoordOut;\n"
                "varying float lightIntensity;\n"
               #endif
                "\n"
                "uniform sampler2D demoTexture;\n"
                "uniform float timeKeeper;\n"
                "uniform vec2 viewResolution;\n"
                "uniform float audioAmplitude;\n"
                "uniform float synthNoteColor;\n\n"
                "vec3 palette(float t)\n"
                "{\n"
                "   vec3 a = vec3(0.6, 0.4, 0.5);\n"
                "   vec3 b = vec3(0.7, 0.3, 0.5);\n"
                "   vec3 c = vec3(0.8, 1.0, 0.6);\n"
                "   vec3 d = vec3(0.76, 0.41, 0.96);\n\n"
                "   return a + b * cos(6.28318 * (c * t + d));\n"
                "}\n"
                "\n"
                "void main()\n"
                "{\n"
               #if JUCE_OPENGL_ES
                "   highp float l = max (0.3, lightIntensity * 0.3);\n"
                "   highp vec4 colour = vec4 (l, l, l, 1.0);\n"
               #else
                "   vec2 uv = (gl_FragCoord.xy * 2.0 - viewResolution.xy) / viewResolution.y;\n"
                "   vec2 uv0 = uv;\n"
                "   vec3 finalColor = vec3(0.0);\n"
                "   for (float i = 0.0; i < 4.0; ++i)\n"
                "   {\n"
                "       uv = fract(uv * 1.6) - 0.5;\n"
                "       float d = length(uv) * exp(-length(uv0));\n"
                "       vec3 color = palette(length(uv0) + i * 0.2 + timeKeeper * 0.4 + synthNoteColor);\n"
                "       d = abs(sin(d*12.0 + timeKeeper + audioAmplitude) / 12.0);\n"
                "       d = pow(0.015 / d, 1.3);\n"
                "       finalColor += (color / 4.0) * d;\n"
                "   }\n"
               #endif
                "    gl_FragColor = vec4(finalColor, 1.0);\n"
                "}\n"
            },
            {
                "Synth Ball",

                SHADER_DEMO_HEADER
                "attribute vec2 position;\n"
                "attribute vec4 normal;\n"
                "attribute vec4 sourceColour;\n"
                "attribute vec2 textureCoordIn;\n"
                "\n"
                "uniform mat4 projectionMatrix;\n"
                "uniform mat4 viewMatrix;\n"
                "uniform vec4 lightPosition;\n"
                "\n"
                "varying vec4 destinationColour;\n"
                "varying vec2 textureCoordOut;\n"
                "varying float lightIntensity;\n"
                "\n"
                "void main()\n"
                "{\n"
                "textureCoordOut = position * 0.5 + 0.5;\n"
                "gl_Position = vec4(position, 0.0, 1.0);\n"
                "}\n",

                SHADER_DEMO_HEADER
               #if JUCE_OPENGL_ES
                "varying lowp vec4 destinationColour;\n"
                "varying lowp vec2 textureCoordOut;\n"
               #else
                "varying vec4 destinationColour;\n"
                "varying vec2 textureCoordOut;\n"
               #endif
                "uniform float timeKeeper;\n"
                "uniform vec2 viewResolution;\n"
                "uniform float audioAmplitude;\n"
                "uniform float synthNoteColor;\n"
                "\n"
                "#define INNER_RADIUS 0.75\n"
                "#define OUTER_RADIUS 0.9\n"
                "#define SHEET_THICKNESS 0.012\n"
                "#define NOISINESS 10.0\n"
                "#define INNER_COLOR vec4(0.0, 30.0, 30.0, 1.0)\n"
                "#define OUTER_COLOR vec4(20.0, 20.0, 30.0, 1.0)\n"
                "#define NUM_STEPS 20\n"
                "#define TIME_SCALE 5.0\n"
                "\n"
                "vec3 palette(float t)\n"
                "{\n"
                "   vec3 a = vec3(0.6, 0.4, 0.5);\n"
                "   vec3 b = vec3(0.7, 0.3, 0.5);\n"
                "   vec3 c = vec3(0.8, 1.0, 0.6);\n"
                "   vec3 d = vec3(0.76, 0.41, 0.96);\n\n"
                "   return a + b * cos(6.28318 * (c * t + d));\n"
                "}\n"
                "\n"
                "float trapezium(float x)\n"
                "{\n"
                "   return min(1.0, max(0.0, 1.0 - abs(-mod(x, 1.0) * 3.0 + 1.0)) * 2.0);\n"
                "}\n"

                "vec3 colFromHue(float hue)\n"
                "{\n"
                    "return vec3(trapezium(hue - 1.0 / 3.0), trapezium(hue), trapezium(hue + 1.0 / 3.0));\n"
                "}\n"
                "\n"
                "float cnoise3(float pos)\n"
                "{\n"
                "   return (cos(pos / 2.0) * 0.2 + 1.0);\n"
                "}\n"
                "\n"
                "float cnoise2(float pos)\n"
                "{\n"
                "    return (sin(pos * cnoise3(pos) / 2.0) * 0.2 + 1.0);\n"
                "}\n"
                "\n"
                "float cnoise(vec4 pos)\n"
                "{\n"
                "   float x = pos.x * cnoise2(pos.y) + pos.w * 0.87123 + 82.52;\n"
                "   float y = pos.y * cnoise2(pos.z) + pos.w * 0.78725 + 12.76;\n"
                "   float z = pos.z * cnoise2(pos.x) + pos.w * 0.68201 + 42.03;\n"
                "   return (sin(x) + sin(y) + sin(z)) / 3.0;\n"
                "}\n"
                "\n"
                "vec4 merge_colours(vec4 apply_this, vec4 on_top_of_this)\n"
                "{\n"
                "    return on_top_of_this * (1.0 - apply_this.a) + apply_this * apply_this.a;\n"
                "}\n"
                "\n"
                "vec4 getdensity(vec3 pos)\n"
                "{\n"
                "    float time = timeKeeper * TIME_SCALE;\n"
                "    vec3 samplePos = normalize(pos);\n"
                "    vec4 inner_color = vec4(colFromHue(cnoise(vec4(samplePos / 5.0, time / 15.0))) * 25.0, 0.5 + (audioAmplitude * 0.5));\n"
                "    inner_color = merge_colours(inner_color, vec4(palette(synthNoteColor), 1.0));\n"
                "    vec4 outer_color = merge_colours(vec4(25.0,25.0,25.0,0.5), inner_color);\n"
                "    float sample_ = (cnoise(vec4(samplePos * NOISINESS, time)) + 1.0) / 2.0;\n"
                "    sample_ = clamp(sample_, 0.0, 1.0);\n"
                "    float innerIncBorder = INNER_RADIUS + SHEET_THICKNESS;\n"
                "    float outerIncBorder = OUTER_RADIUS - SHEET_THICKNESS;\n"
                "    float radius = innerIncBorder + (outerIncBorder - innerIncBorder) * sample_;\n"
                "    float dist = distance(pos, vec3(0.0, 0.0, 0.0));\n"
                "    float density = exp(-pow(dist - radius, 2.0) * 05000.0) * 0.7 + (audioAmplitude * 0.3);\n"
                "    return (inner_color + (outer_color - inner_color) * (radius - innerIncBorder) / (outerIncBorder - innerIncBorder)) * density;\n"
                "}\n"
                "\n"
                "vec4 raymarch(vec3 start, vec3 end)\n"
                "{\n"
                "    vec4 retn = vec4(0.0, 0.0, 0.0, 0.0);\n"
                "    vec3 delta = end - start;\n"
                "    float stepDistance = length(delta) / float(NUM_STEPS);\n"

                "    vec4 densityPrevious = getdensity(start);\n"
                "    for (int i = 1; i < NUM_STEPS; i++)\n"
                "    {\n"
                "        vec3 samplePos = start + delta * float(i) / float(NUM_STEPS);\n"
                "        vec4 density = getdensity(samplePos);\n"
                "        vec4 densityIntegrated = (density + densityPrevious) / 2.0;\n"
                "        retn += densityIntegrated;\n"
                "        densityPrevious = density;\n"
                "    }\n"
                "    return retn * stepDistance;\n"
                "}\n"
                "\n"
                "vec4 raymarch_ball(vec2 coord)\n"
                "{\n"
                "    float d = distance(coord, vec2(0.0, 0.0));\n"
                "    if (d > OUTER_RADIUS) {\n"
                "        return vec4(0.0, 0.0, 0.0, 0.0);\n"
                "    }\n"
                "    float dOuterNormalized = d / OUTER_RADIUS;\n"
                "    float outerStartZ = -sqrt(1.0 - dOuterNormalized * dOuterNormalized) * OUTER_RADIUS;\n"
                "    float outerEndZ = -outerStartZ;\n"
                "    if (d > INNER_RADIUS) {\n"
                "        vec4 frontPart = raymarch(vec3(coord, outerStartZ), vec3(coord, 0));\n"
                "        vec4 backPart = raymarch(vec3(coord, 0), vec3(coord, outerEndZ));\n"
                "        return frontPart + backPart;\n"
                "    }\n"

                "    float dInnerNormalized = d / INNER_RADIUS;\n"
                "    float innerStartZ = -sqrt(1.0 - dInnerNormalized * dInnerNormalized) * INNER_RADIUS;\n"
                "    float innerEndZ = -innerStartZ;\n"
                "    vec4 frontPart = raymarch(vec3(coord, outerStartZ), vec3(coord, innerStartZ));\n"
                "    vec4 backPart = raymarch(vec3(coord, innerEndZ), vec3(coord, outerEndZ));\n"
                "    vec4 final = frontPart + backPart;\n"
                "    return final;\n"
                "}\n"
                "\n"

                "void main()\n"
                "{\n"
                "    vec2 uv = (gl_FragCoord.xy / min(viewResolution.x, viewResolution.y)) * 2.0 - vec2(viewResolution.x / viewResolution.y, 1.0);\n"
                "    gl_FragColor = merge_colours(raymarch_ball(uv), vec4(0.0, 0.0, 0.0, 1.0));\n"
                "}\n"
            }
        };

        return Array<ShaderPreset> (presets, numElementsInArray (presets));
    }
};

class OpenGLComponent  : public Component
                       , private OpenGLRenderer
                       , private AsyncUpdater
                       , private Timer
{
public:
    OpenGLComponent()
    {
        if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine (0);

        setOpaque (true);
        controlsOverlay.reset (new OpenGLComponentControlsOverlay (*this));
        addAndMakeVisible (controlsOverlay.get());

        openGLContext.setRenderer (this);
        openGLContext.attachTo (*this);
        openGLContext.setContinuousRepainting (true);

        controlsOverlay->initialise();

        startTimer(30);
        setSize (500, 500);
    }

    ~OpenGLComponent() override
    {
        openGLContext.detach();
    }

    void timerCallback() override
    {
        time += 0.030f;
        triggerAsyncUpdate();
    }

    void newOpenGLContextCreated() override
    {
        freeAllContextObjects();

        if (controlsOverlay.get() != nullptr)
            controlsOverlay->updateShader();
    }

    void openGLContextClosing() override
    {
        freeAllContextObjects();
    }

    void freeAllContextObjects()
    {
        shape     .reset();
        shader    .reset();
        attributes.reset();
        uniforms  .reset();
    }

    void renderOpenGL() override
    {
        using namespace ::juce::gl;

        const ScopedLock lock (mutex);

        jassert (OpenGLHelpers::isContextActive());

        auto desktopScale = (float) openGLContext.getRenderingScale();

        OpenGLHelpers::clear (juce::Colours::black);

        // First draw our background graphics to demonstrate the OpenGLGraphicsContext class
        if (doBackgroundDrawing)
            drawBackground2DStuff (desktopScale);

        updateShader();   // Check whether we need to compile a new shader

        if (shader.get() == nullptr)
            return;

        // Having used the juce 2D renderer, it will have messed-up a whole load of GL state, so
        // we need to initialise some important settings before doing our normal GL 3D drawing..
        glEnable (GL_DEPTH_TEST);
        glDepthFunc (GL_LESS);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture (GL_TEXTURE0);
        glEnable (GL_TEXTURE_2D);

        glViewport (0, 0,
                    roundToInt (desktopScale * (float) bounds.getWidth()),
                    roundToInt (desktopScale * (float) bounds.getHeight()));

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        shader->use();

        if (uniforms->projectionMatrix.get() != nullptr)
            uniforms->projectionMatrix->setMatrix4 (getProjectionMatrix().mat, 1, false);

        if (uniforms->viewMatrix.get() != nullptr)
            uniforms->viewMatrix->setMatrix4 (getViewMatrix().mat, 1, false);

        if (uniforms->texture.get() != nullptr)
            uniforms->texture->set ((GLint) 0);

        if (uniforms->lightPosition.get() != nullptr)
            uniforms->lightPosition->set (-15.0f, 10.0f, 15.0f, 0.0f);

        if (uniforms->bouncingNumber.get() != nullptr)
            uniforms->bouncingNumber->set (bouncingNumber.getValue());

        if (uniforms->viewResolution.get() != nullptr)
            uniforms->viewResolution->set (getWidth(), getHeight());

        if (uniforms->timeKeeper.get() != nullptr)
            uniforms->timeKeeper->set(time);

        if (uniforms->audioAmplitude.get() != nullptr)
            uniforms->audioAmplitude->set(colorIntensity);

        if (uniforms->synthNoteColor.get() != nullptr)
            uniforms->synthNoteColor->set(colorVal);

        shape->draw (*attributes);

        // Reset the element buffers so child Components draw correctly
        glBindBuffer (GL_ARRAY_BUFFER, 0);
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

        if (! controlsOverlay->isMouseButtonDownThreadsafe())
            rotation += (float) rotationSpeed;
    }

    Matrix3D<float> getProjectionMatrix() const
    {
        const ScopedLock lock (mutex);

        auto w = 1.0f / (scale + 0.1f);
        auto h = w * bounds.toFloat().getAspectRatio (false);

        return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
    }

    Matrix3D<float> getViewMatrix() const
    {
        const ScopedLock lock (mutex);

        auto viewMatrix = Matrix3D<float>::fromTranslation ({ 0.0f, 1.0f, -10.0f }) * draggableOrientation.getRotationMatrix();
        auto rotationMatrix = Matrix3D<float>::rotation ({ rotation, rotation, -0.3f });

        return viewMatrix * rotationMatrix;
    }

    void setShaderProgram (const String& vertexShader, const String& fragmentShader)
    {
        const ScopedLock lock (shaderMutex); // Prevent concurrent access to shader strings and status
        newVertexShader = vertexShader;
        newFragmentShader = fragmentShader;
    }



    void paint(Graphics& g) override {}

    void resized() override
    {
        const ScopedLock lock (mutex);

        bounds = getLocalBounds();
        controlsOverlay->setBounds (bounds);
        //draggableOrientation.setViewport (bounds);
    }

    Rectangle<int> bounds;
    Draggable3DOrientation draggableOrientation;
    bool doBackgroundDrawing = false;
    float scale = 0.5f, rotationSpeed = 0.0f;
    BouncingNumber bouncingNumber;
    CriticalSection mutex;
    float time = 0.0f, colorIntensity = 1.0f, colorVal = 0.0f;

private:
    void handleAsyncUpdate() override
    {
        const ScopedLock lock (shaderMutex); // Prevent concurrent access to shader strings and status
        controlsOverlay->statusLabel.setText (statusText, dontSendNotification);
    }

    void drawBackground2DStuff (float desktopScale)
    {
        // Create an OpenGLGraphicsContext that will draw into this GL window..
        std::unique_ptr<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (openGLContext,
                                                                                          roundToInt (desktopScale * (float) bounds.getWidth()),
                                                                                          roundToInt (desktopScale * (float) bounds.getHeight())));

        if (glRenderer.get() != nullptr)
        {
            Graphics g (*glRenderer);
            g.addTransform (AffineTransform::scale (desktopScale));

            for (auto s : stars)
            {
                auto size = 0.25f;

                // This stuff just creates a spinning star shape and fills it..
                Path p;
                p.addStar ({ (float) bounds.getWidth()  * s.x.getValue(),
                             (float) bounds.getHeight() * s.y.getValue() },
                           7,
                           (float) bounds.getHeight() * size * 0.5f,
                           (float) bounds.getHeight() * size,
                           s.angle.getValue());

                auto hue = s.hue.getValue();

                g.setGradientFill (ColourGradient (Colours::green.withRotatedHue (hue).withAlpha (0.8f),
                                                   0, 0,
                                                   Colours::red.withRotatedHue (hue).withAlpha (0.5f),
                                                   0, (float) bounds.getHeight(), false));
                g.fillPath (p);
            }
        }
    }

    OpenGLContext openGLContext;

    //==============================================================================
    /**
        This component sits on top of the main GL demo, and contains all the sliders
        and widgets that control things.
    */
    class OpenGLComponentControlsOverlay  : public Component
                                          , private CodeDocument::Listener
                                          , private Timer
    {
    public:
        OpenGLComponentControlsOverlay (OpenGLComponent& d)
            : demo (d)
        {
            addAndMakeVisible (statusLabel);
            statusLabel.setJustificationType (Justification::topLeft);
            statusLabel.setFont (Font (14.0f));

            addAndMakeVisible (tabbedComp);
            tabbedComp.setTabBarDepth (25);
            tabbedComp.setColour (TabbedButtonBar::tabTextColourId, Colours::grey);
            tabbedComp.addTab ("Vertex", Colours::transparentBlack, &vertexEditorComp, false);
            tabbedComp.addTab ("Fragment", Colours::transparentBlack, &fragmentEditorComp, false);

            vertexDocument.addListener (this);
            fragmentDocument.addListener (this);

            addAndMakeVisible (presetBox);
            presetBox.onChange = [this] { selectPreset (presetBox.getSelectedItemIndex()); };

            auto presets = OpenGLUtils::getPresets();

            for (int i = 0; i < presets.size(); ++i)
                presetBox.addItem (presets[i].name, i + 1);

            addAndMakeVisible (presetLabel);
            presetLabel.attachToComponent (&presetBox, false);

            lookAndFeelChanged();
        }

        void initialise()
        {
            presetBox .setSelectedItemIndex (0);
        }

        void resized() override
        {
            presetBox.setBounds (getWidth() - 120, 0, 120, 30);
        }

        bool isMouseButtonDownThreadsafe() const { return buttonDown; }

        void mouseDown (const MouseEvent& e) override
        {
            const ScopedLock lock (demo.mutex);
            demo.draggableOrientation.mouseDown (e.getPosition());

            buttonDown = true;
        }

        void mouseDrag (const MouseEvent& e) override
        {
            const ScopedLock lock (demo.mutex);
            demo.draggableOrientation.mouseDrag (e.getPosition());
        }

        void mouseUp (const MouseEvent&) override
        {
            buttonDown = false;
        }

        void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& d) override
        {
            sizeSlider.setValue (sizeSlider.getValue() + d.deltaY);
        }

        void mouseMagnify (const MouseEvent&, float magnifyAmmount) override
        {
            sizeSlider.setValue (sizeSlider.getValue() + magnifyAmmount - 1.0f);
        }

        void selectPreset (int preset)
        {
            const auto& p = OpenGLUtils::getPresets()[preset];

            vertexDocument  .replaceAllContent (p.vertexShader);
            fragmentDocument.replaceAllContent (p.fragmentShader);

            startTimer (1);
        }

        void updateShader()
        {
            startTimer (10);
        }

        Label statusLabel;

    private:
        enum { shaderLinkDelay = 500 };

        void codeDocumentTextInserted (const String& /*newText*/, int /*insertIndex*/) override
        {
            startTimer (shaderLinkDelay);
        }

        void codeDocumentTextDeleted (int /*startIndex*/, int /*endIndex*/) override
        {
            startTimer (shaderLinkDelay);
        }

        void timerCallback() override
        {
            stopTimer();
            demo.setShaderProgram (vertexDocument  .getAllContent(),
                                   fragmentDocument.getAllContent());
        }

        void lookAndFeelChanged() override
        {
            auto editorBackground = getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground,
                                                            Colours::white);

            for (int i = tabbedComp.getNumTabs(); i >= 0; --i)
                tabbedComp.setTabBackgroundColour (i, editorBackground);

            vertexEditorComp  .setColour (CodeEditorComponent::backgroundColourId, editorBackground);
            fragmentEditorComp.setColour (CodeEditorComponent::backgroundColourId, editorBackground);
        }

        OpenGLComponent& demo;

        Label speedLabel  { {}, "Speed:" },
              zoomLabel   { {}, "Zoom:" };

        CodeDocument vertexDocument, fragmentDocument;
        CodeEditorComponent vertexEditorComp    { vertexDocument,   nullptr },
                            fragmentEditorComp  { fragmentDocument, nullptr };

        TabbedComponent tabbedComp              { TabbedButtonBar::TabsAtLeft };

        ComboBox presetBox, textureBox;

        Label presetLabel   { {}, "Shader Preset:" };

        Slider speedSlider, sizeSlider;

        ToggleButton showBackgroundToggle  { "Draw 2D graphics in background" };

        std::unique_ptr<FileChooser> textureFileChooser;

        std::atomic<bool> buttonDown { false };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponentControlsOverlay)
    };

    std::unique_ptr<OpenGLComponentControlsOverlay> controlsOverlay;

    float rotation = 0.0f;

    std::unique_ptr<OpenGLShaderProgram> shader;
    std::unique_ptr<OpenGLUtils::Shape> shape;
    std::unique_ptr<OpenGLUtils::Attributes> attributes;
    std::unique_ptr<OpenGLUtils::Uniforms> uniforms;

    CriticalSection shaderMutex;
    String newVertexShader, newFragmentShader, statusText;

    struct BackgroundStar
    {
        SlowerBouncingNumber x, y, hue, angle;
    };

    BackgroundStar stars[3];

    //==============================================================================
    void updateShader()
    {
        const ScopedLock lock (shaderMutex); // Prevent concurrent access to shader strings and status

        if (newVertexShader.isNotEmpty() || newFragmentShader.isNotEmpty())
        {
            std::unique_ptr<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));

            if (newShader->addVertexShader (OpenGLHelpers::translateVertexShaderToV3 (newVertexShader))
                  && newShader->addFragmentShader (OpenGLHelpers::translateFragmentShaderToV3 (newFragmentShader))
                  && newShader->link())
            {
                shape     .reset();
                attributes.reset();
                uniforms  .reset();

                shader.reset (newShader.release());
                shader->use();

                shape     .reset (new OpenGLUtils::Shape      ());
                attributes.reset (new OpenGLUtils::Attributes (*shader));
                uniforms  .reset (new OpenGLUtils::Uniforms   (*shader));

                statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
            }
            else
            {
                statusText = newShader->getLastError();
            }

            triggerAsyncUpdate();

            newVertexShader   = {};
            newFragmentShader = {};
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponent)
};
