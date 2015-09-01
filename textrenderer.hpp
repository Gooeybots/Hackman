#ifndef TEXTRENDERER_HPP
#define TEXTRENDERER_HPP

#include <memory>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class TextRenderer
{
public:
    enum Alignment
    {
        Top,
        Bottom,
        Left,
        Right,
        Center
    };

    TextRenderer(const char * textureFilename = nullptr);
    ~TextRenderer();

    void DrawAll();
    void Draw(const std::string &text);
// The offset is from the left/bottom of the text
    void AddText(const std::string &text, const float xOffset, const Alignment vertical,
                 const float size, const glm::vec4 &colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    void AddText(const std::string &text, const Alignment horizontal, const float yOffset,
                 const float size, const glm::vec4 &colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
// Adds text to the renderer at a certain alignment always stays at that alignment until it is changed
    void AddText(const std::string &text, const Alignment horizontal, const Alignment vertical,
                 const float size, const glm::vec4 &colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
// Adds text to the renderer aligned seperated from top to bottom
    void AddTextVerticalAlign(const std::vector<std::string> &text, const Alignment horizontal, const Alignment vertical,
                 const float size, const glm::vec4 &colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
// Same as above but from left to right
    void AddTextHorizontalAlign(const std::vector<std::string> &text, const Alignment horizontal, const Alignment vertical,
                                const float size, const glm::vec4 &colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
// Adds text to the renderer at a certain position in pixels from the bottom left
    void AddTextBottomLeft(const std::string &text, const float leftOffset, const float bottomOffset,
                           const float size, const glm::vec4 &colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    void AddTextTopRight(const std::string &text, const float rightOffset, const float topOffset,
                         const float size, const glm::vec4 &colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

// Removes the text from the vector
    void RemoveText(const std::string &text);

// Moves text if it exists in vector from posititon in pixles
    void MoveTextWithAlignment(const std::string &text, const Alignment horizontal, const Alignment vertical);
    void MoveTextVerticallyFromTop(const std::string &text, const float whereFromTop);
    void MoveTextVerticallyFromBottom(const std::string &text, const float whereFromBottom);
    void MoveTextVerticallyWithAlignment(const std::string &text, const Alignment alignment);
    void MoveTextHorizontallyFromLeft(const std::string &text, const float whereFromLeft);
    void MoveTextHorizontallyFromRight(const std::string &text, const float whereFromRight);
    void MoveTextHorizontallyWithAlignment(const std::string &text, const Alignment alignment);

// Changes text colour if it is in vector
    void ChangeTextColour(const std::string &text, const glm::vec4 &colour);
    void ChangeAllTextColour(const glm::vec4 &colour);

// Changes the text size
    void ChangeTextSize(const std::string &text, const float size);
    void ChangeAllTextSizes(const float size);
    void IncrementTextSize(const std::string &text, const float increment);
    void IncrementAllTextSize(const float increment);

private:
    class Text
    {
    public:
        Text(const std::string &text, const glm::vec4 &colour, const float xOffset,
             const float yOffset, const float size);
        ~Text();

        glm::mat4 mModel;
        glm::vec4 mColour;
        std::string mText;
        float mSize;
        float mXOffset;
        float mYOffset;
        unsigned int mVao;
        unsigned int mVbo;

        void UpdateModel();
    private:
        void CreateVao(const std::string &text);
        void CreateVbo(const std::string &text, std::vector<glm::vec2> &posVec);
        void AddTexCoords(const std::string &text, std::vector<glm::vec2> &posVec);
        void GetTexCoordLocation(const char &letter, float &x, float&y);
    };

    std::vector<std::shared_ptr<Text>>::iterator GetPositionOfText(const std::string &text);
    float AlignVertically(const Alignment vertAlign, const float size);
    float AlignHorizontally(const Alignment horiAlign, const size_t stringLength, const float size);
    float GetSizeOfAllText(const std::vector<std::string> &text);
    float GetOffsetForCenterVec(const std::vector<std::string> &text, const float size);
    bool IsTextInVector(const std::string &text);
    void Draw(const Text &text);
    void UpdateSizeAndPosition(Text &text, const float size);

    std::vector<std::shared_ptr<Text>> mTextVec;
    glm::mat4 mView;
    unsigned int mProgram;
    unsigned int mTexture;
    int mWidth;
    int mHeight;
};

#endif // TEXTRENDERER_HPP
