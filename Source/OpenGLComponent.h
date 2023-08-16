#pragma once
#include "BatSynthAudioProcessor.h"
#include "BatSynthAudioProcessorEditor.h"

//==============================================================================
class OpenGLComponent : public Component
                      , private CodeDocument::Listener
                      , private Timer
{
public:
    OpenGLComponent()
    {
        setOpaque(true);

        if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine(0);

        openGLContext.attachTo(*getTopLevelComponent());
        addAndMakeVisible(statusLabel);
        statusLabel.setJustificationType(Justification::topLeft);
        statusLabel.setFont(Font(14.0f));

        auto presets = getPresets();

        for (int i = 0; i < presets.size(); ++i)
            presetBox.addItem(presets[i].name, i + 1);

        addAndMakeVisible(presetLabel);
        presetLabel.attachToComponent(&presetBox, true);

        addAndMakeVisible(presetBox);
        presetBox.onChange = [this] { selectPreset(presetBox.getSelectedItemIndex()); };

        fragmentEditorComp.setOpaque(false);
        fragmentDocument.addListener(this);
        addAndMakeVisible(fragmentEditorComp);

        presetBox.setSelectedItemIndex(0);

        setVisible(true);
        openGLContext.makeActive();

        setSize(500, 500);
    }

    ~OpenGLComponent() override
    {
        openGLContext.detach();
        shader.reset();
    }

    void paint(Graphics& g) override
    {
        g.fillCheckerBoard(getLocalBounds().toFloat(), 48.0f, 48.0f, Colours::lightgrey, Colours::white);
        g.fillRoundedRectangle({ 0.f, 0.f, static_cast<float>(getWidth()), static_cast<float>(getHeight()) }, 20);
        if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
        {
            shader.reset();

            if (fragmentCode.isNotEmpty())
            {
                shader.reset(new OpenGLGraphicsContextCustomShader(fragmentCode));

                auto result = shader->checkCompilation(g.getInternalContext());

                if (result.failed())
                {
                    statusLabel.setText(result.getErrorMessage(), dontSendNotification);
                    shader.reset();
                    isShaderCompiledSuccessfully = false;
                    return;
                }
                else
                    isShaderCompiledSuccessfully = true;
            }
        }

        if (shader.get() != nullptr && isShaderCompiledSuccessfully)
        {
            shader->getProgram(g.getInternalContext())->setUniform("audioAmplitude", audioAmplitude);
            shader->getProgram(g.getInternalContext())->setUniform("audioNoteColor", audioNoteColor);
            shader->fillRect(g.getInternalContext(), getLocalBounds());
        }
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(4);

        statusLabel.setBounds(area.removeFromTop(75));

        area.removeFromTop(area.getHeight() / 2);

        auto presets = area.removeFromTop(25);
        presets.removeFromLeft(100);
        presetBox.setBounds(presets.removeFromLeft(150));

        area.removeFromTop(4);
        fragmentEditorComp.setBounds(area);

    }

    void selectPreset(int preset)
    {
        fragmentDocument.replaceAllContent(getPresets()[preset].fragmentShader);
        startTimer (1);
        isShaderCompiledSuccessfully = false;
    }

    void setShaderColor(float newColor)
    {
        if (shader != nullptr)
            audioNoteColor = newColor;
    }

    void setShaderColorIntensity(float newIntensity)
    {
        if (shader != nullptr)
            audioAmplitude = newIntensity;
    }

    std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;

    Label statusLabel, presetLabel { {}, "Shader Preset:" };
    ComboBox presetBox;
    CodeDocument fragmentDocument;
    CodeEditorComponent fragmentEditorComp { fragmentDocument, nullptr };
    String fragmentCode;

private:
    OpenGLContext openGLContext;
    bool isShaderCompiledSuccessfully = false;
    float audioNoteColor, audioAmplitude;

    enum { shaderLinkDelay = 500 };

    void codeDocumentTextInserted(const String& /*newText*/, int /*insertIndex*/) override
    {
        startTimer(shaderLinkDelay);
    }

    void codeDocumentTextDeleted(int /*startIndex*/, int /*endIndex*/) override
    {
        startTimer(shaderLinkDelay);
    }

    void timerCallback() override
    {
        stopTimer();
        fragmentCode = fragmentDocument.getAllContent();
        repaint();
    }

    struct ShaderPreset
    {
        const char* name;
        const char* fragmentShader;
    };

    static Array<ShaderPreset> getPresets()
    {
#define SHADER_2DDEMO_HEADER \
            "/*  This editor shows the use of the OpenGLGraphicsContextCustomShader,\n" \
            "    which allows a 2D area to be filled using a GL shader program.\n" \
            "\n" \
            "    Edit the shader program below and it will be \n" \
            "    recompiled in real-time!\n" \
            "*/\n\n"

        ShaderPreset presets[] =
        {
            {
                "Simple Gradient",
                SHADER_2DDEMO_HEADER
                "uniform float audioAmplitude;\n"
                "uniform float audioNoteColor;\n"
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0 * audioAmplitude, 0.4, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " float alpha = pixelPos.x / 1000.0;\n"
                "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
                "}\n"
            },
            {
                "Circular Gradient",
                SHADER_2DDEMO_HEADER
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.3, 0.4, 0.4, 1.0);\n"
                "    " JUCE_MEDIUMP " float alpha = distance (pixelPos, vec2 (600.0, 500.0)) / 400.0;\n"
                "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
                "}\n"
            },
            {
                "Circle",
                SHADER_2DDEMO_HEADER
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (0.1, 0.1, 0.9, 1.0);\n"
                "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
                "    " JUCE_MEDIUMP " float distance = distance (pixelPos, vec2 (600.0, 500.0));\n"
                "\n"
                "    " JUCE_MEDIUMP " float innerRadius = 200.0;\n"
                "    " JUCE_MEDIUMP " float outerRadius = 210.0;\n"
                "\n"
                "    if (distance < innerRadius)\n"
                "        gl_FragColor = colour1;\n"
                "    else if (distance > outerRadius)\n"
                "        gl_FragColor = colour2;\n"
                "    else\n"
                "        gl_FragColor = mix (colour1, colour2, (distance - innerRadius) / (outerRadius - innerRadius));\n"
                "\n"
                "    gl_FragColor *= pixelAlpha;\n"
                "}\n"
            },
            {
                "Radial Pattern",

                SHADER_2DDEMO_HEADER
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec2 center = vec2(500.0, 400.0);\n"
                "    " JUCE_MEDIUMP " vec2 pos = pixelPos.xy - center;\n"
                "    " JUCE_MEDIUMP " float angle = atan(pos.y, pos.x);\n"
                "    " JUCE_MEDIUMP " float distanceFromCenter = length(pos) / 300.0;\n"
                "    " JUCE_MEDIUMP " float checker = mod(floor(10.0 * angle / 3.14159) + floor(10.0 * distanceFromCenter), 2.0);\n"
                "    gl_FragColor = vec4(checker, checker, checker, 1.0) * pixelAlpha;\n"
                "}\n"
            },
            {
                "Noise Texture",

                SHADER_2DDEMO_HEADER
                "uniform float audioAmplitude;\n"
                "uniform float audioNoteColor;\n\n"
                "void main()\n"
                "{\n"
                "    " JUCE_MEDIUMP " vec2 pos = pixelPos.xy / 50.0 * audioAmplitude;\n"
                "    " JUCE_MEDIUMP " float noise = fract(sin(dot(pos ,vec2(12.9898,78.233))) * 43758.5453);\n"
                "    gl_FragColor = vec4(vec3(noise) * audioNoteColor, 1.0) * pixelAlpha;\n"
                "}\n"
            }
        };

        return Array<ShaderPreset>(presets, numElementsInArray(presets));
    }


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLComponent)
};
