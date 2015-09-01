#define GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "loadtexture.hpp"
#include "textrenderer.hpp"
#include "setupprogram.hpp"

TextRenderer::TextRenderer(const char *textureFilename)
{
    glfwGetWindowSize(glfwGetCurrentContext(), &mWidth, &mHeight);
    float width(mWidth * 1.0f), height(mHeight * 1.0f);
    mTexture = LoadTexture(!textureFilename ? "text.png" : textureFilename);
    mProgram = SetupProgram("text.vs", "text.fs");
    mView = glm::ortho(0.0f, width * 1.0f, 0.0f, height * 1.0f, 1.0f, -1.0f);
}

TextRenderer::~TextRenderer()
{
    if(glIsTexture(mTexture))
        glDeleteTextures(1, &mTexture);
    if(glIsProgram(mProgram))
        glDeleteProgram(mProgram);
}

void TextRenderer::DrawAll()
{
    if(!mTextVec.empty())
    {
        glUseProgram(mProgram);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        for(auto & text : mTextVec)
        {
            Draw(*text);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }
}

void TextRenderer::Draw(const std::string &text)
{
    std::vector<std::shared_ptr<Text>>::iterator it(GetPositionOfText(text));
    if(it != mTextVec.end())
    {
        glUseProgram(mProgram);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        Draw(*(*it));
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }
}

void TextRenderer::AddText(const std::string &text, const Alignment horiAlign,
                           const float yOffset, const float size, const glm::vec4 &colour)
{
    if(!IsTextInVector(text))
    {
        float halfSize(size * 0.5f);
        float xOffset(AlignHorizontally(horiAlign, text.size(), size));
        std::shared_ptr<Text> tPoint(new Text(text, colour, xOffset, yOffset + halfSize, size));
        mTextVec.push_back(tPoint);
    }
}

void TextRenderer::AddText(const std::string &text, const float xOffset,
                           const Alignment vertAlign, const float size, const glm::vec4 &colour)
{
    if(!IsTextInVector(text))
    {
        float halfSize((text.size() * size) * 0.5f);
        float yOffset(AlignVertically(vertAlign, size));
        std::shared_ptr<Text> tPoint(new Text(text, colour, xOffset + halfSize, yOffset, size));
        mTextVec.push_back(tPoint);
    }
}

void TextRenderer::AddText(const std::string &text, const Alignment horiAlign,
                           const Alignment vertAlign, const float size, const glm::vec4 &colour)
{
    if(!IsTextInVector(text))
    {
        float xOffset(AlignHorizontally(horiAlign, text.size(), size));
        float yOffset(AlignVertically(vertAlign, size));
        std::shared_ptr<Text> tPoint(new Text(text, colour, xOffset, yOffset, size));
        mTextVec.push_back(tPoint);
    }
}

void TextRenderer::AddTextVerticalAlign(const std::vector<std::string> &textVec, const Alignment horizontal,
                                        const Alignment vertical, const float size, const glm::vec4 &colour)
{
    float offsetSize(0.0f);
    switch(vertical)
    {
    case Alignment::Center:
        offsetSize = (textVec.size() / 2.0f) * size;
        break;
    case Alignment::Bottom:
        offsetSize = (textVec.size() * size) - size;
        break;
    case Alignment::Top:
    case Alignment::Left:
    case Alignment::Right:
    default:
        break;
    }

    for(auto & text : textVec)
    {
        std::vector<std::shared_ptr<TextRenderer::Text>>::iterator textIt(GetPositionOfText(text));
        if(textIt == mTextVec.end())
        {
            float xOffset(AlignHorizontally(horizontal, text.size(), size));
            float yOffset(AlignVertically(vertical, size));
            yOffset += offsetSize;
            std::shared_ptr<Text> tPoint(new Text(text, colour, xOffset, yOffset, size));
            mTextVec.push_back(tPoint);
        }
        else
        {
            (*textIt)->mXOffset = AlignHorizontally(horizontal, text.size(), size);
            (*textIt)->mYOffset = AlignVertically(vertical, size);
            (*textIt)->mYOffset += offsetSize;
        }
        offsetSize -= size;
    }
}

void TextRenderer::AddTextHorizontalAlign(const std::vector<std::string> &textVec, const Alignment horizontal,
                                          const Alignment vertical, const float size, const glm::vec4 &colour)
{
    float offsetSize(0.0f);
    switch(horizontal)
    {
    case Alignment::Center:
        offsetSize = AlignHorizontally(horizontal, 0, 0);
        offsetSize += GetOffsetForCenterVec(textVec, size);
        break;
    case Alignment::Right:
        offsetSize = -(GetSizeOfAllText(textVec) * size);
        break;
    case Alignment::Left:
    case Alignment::Top:
    case Alignment::Bottom:
    default:
        break;
    }

    for(auto text(textVec.begin()); text != textVec.end(); ++text)
    {
        std::shared_ptr<Text> tPoint(nullptr);

        if(horizontal == Alignment::Right)
            offsetSize += (*text).size() * size;

        auto textIt(GetPositionOfText(*text));
        if(textIt == mTextVec.end())
        {
            float xOffset(AlignHorizontally(horizontal, (*text).size(), size));
            xOffset += offsetSize;
            float yOffset(AlignVertically(vertical, size));

            if(horizontal == Alignment::Center)
                tPoint = std::make_shared<Text>((*text), colour, offsetSize, yOffset, size);
            else
                tPoint = std::make_shared<Text>((*text), colour, xOffset, yOffset, size);

            mTextVec.push_back(tPoint);
        }
        else
        {
            if(horizontal == Alignment::Center)
                (*textIt)->mXOffset = offsetSize;
            else
            {
                (*textIt)->mXOffset = AlignHorizontally(horizontal, (*text).size(), size);
                (*textIt)->mXOffset += offsetSize;
            }

            (*textIt)->mYOffset = AlignVertically(vertical, size);
            (*textIt)->mColour = colour;
            (*textIt)->mSize = size;
            (*textIt)->UpdateModel();
        }

        if(horizontal == Alignment::Left)
            offsetSize += (*text).size() * size;
        else if(horizontal == Alignment::Center)
        {
            offsetSize += (((*text).size() * size) * 0.5f);
            if(text + 1 != textVec.end())
                offsetSize += (((*(text+1)).size() * size) * 0.5f);
        }
    }
}

void TextRenderer::AddTextBottomLeft(const std::string &text, const float leftOffset,
                                     const float bottomOffset, const float size, const glm::vec4 &colour)
{
    if(!IsTextInVector(text))
    {
        float halfSize(size * 0.5f);
        float xOffset(leftOffset + (text.size() * 0.5f) + halfSize);
        float yOffset(bottomOffset + halfSize);
        std::shared_ptr<Text> tPoint(new Text(text, colour, xOffset, yOffset, size));
        mTextVec.push_back(tPoint);
    }
}

void TextRenderer::AddTextTopRight(const std::string &text, const float rightOffset,
                                   const float topOffset, const float size, const glm::vec4 &colour)
{
    if(!IsTextInVector(text))
    {
        float halfSize(size * 0.5f);
        float xOffset(mWidth - (rightOffset + (text.size() * 0.5f) + halfSize));
        float yOffset(mHeight - (topOffset + halfSize));
        std::shared_ptr<Text> tPoint(new Text(text, colour, xOffset, yOffset, size));
        mTextVec.push_back(tPoint);
    }
}

void TextRenderer::RemoveText(const std::string &text)
{
    std::vector<std::shared_ptr<Text>>::iterator where(GetPositionOfText(text));
    if(where != mTextVec.end())
        mTextVec.erase(where);
}

void TextRenderer::MoveTextWithAlignment(const std::string &text, const Alignment hori, const Alignment vert)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        float yOffset(AlignVertically(vert, (*textIt)->mSize));
        (*textIt)->mYOffset = yOffset;
        float xOffset(AlignHorizontally(hori, text.size(), (*textIt)->mSize));
        (*textIt)->mXOffset = xOffset;
        (*textIt)->UpdateModel();
    }
}

void TextRenderer::MoveTextVerticallyFromTop(const std::string &text, const float whereFromTop)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        float halfSize((*textIt)->mSize * 0.5f);
        float yOffset(mHeight - (whereFromTop + halfSize));
        (*textIt)->mYOffset = yOffset;
        (*textIt)->UpdateModel();
    }
}

void TextRenderer::MoveTextVerticallyFromBottom(const std::string &text, const float whereFromBottom)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        float halfSize((*(*textIt)).mSize * 0.5f);
        float yOffset(whereFromBottom + halfSize);
        (*textIt)->mYOffset = yOffset;
        (*textIt)->UpdateModel();
    }
}

void TextRenderer::MoveTextVerticallyWithAlignment(const std::string &text,
                                                   const Alignment alignment)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        float yOffset(AlignVertically(alignment, (*textIt)->mSize));
        (*textIt)->mYOffset = yOffset;
        (*textIt)->UpdateModel();
    }
}

void TextRenderer::MoveTextHorizontallyFromLeft(const std::string &text, const float whereFromLeft)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        float halfSize((*textIt)->mSize * 0.5f);
        float xOffset(whereFromLeft + halfSize);
        (*textIt)->mXOffset = xOffset;
        (*textIt)->UpdateModel();
    }
}

void TextRenderer::MoveTextHorizontallyFromRight(const std::string &text, const float whereFromRight)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        float halfSize((*textIt)->mSize * 0.5f);
        float xOffset(mWidth - (whereFromRight + halfSize));
        (*textIt)->mXOffset = xOffset;
        (*textIt)->UpdateModel();
    }
}

void TextRenderer::MoveTextHorizontallyWithAlignment(const std::string &text, const Alignment alignment)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        float xOffset(AlignHorizontally(alignment, (*(*textIt)).mText.size(), (*(*textIt)).mSize));
        (*textIt)->mXOffset = xOffset;
        (*textIt)->UpdateModel();
    }
}

void TextRenderer::ChangeTextColour(const std::string &text, const glm::vec4 &colour)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        (*textIt)->mColour = colour;
    }
}

void TextRenderer::ChangeAllTextColour(const glm::vec4 &colour)
{
    if(!mTextVec.empty())
    {
        for(auto textIt(mTextVec.begin()); textIt != mTextVec.end(); ++textIt)
        {
            (*textIt)->mColour = colour;
        }
    }
}

void TextRenderer::ChangeTextSize(const std::string &text, const float size)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        UpdateSizeAndPosition(*(*textIt), size);
    }
}

void TextRenderer::ChangeAllTextSizes(const float size)
{
    for(auto & text : mTextVec)
        UpdateSizeAndPosition(*text, size);
}

TextRenderer::Text::Text(const std::string &text, const glm::vec4 &colour, const float xOffset,
     const float yOffset, const float size)
    : mColour(colour), mText(text), mSize(size), mXOffset(xOffset), mYOffset(yOffset)
{
    CreateVao(text);
    UpdateModel();
}

void TextRenderer::IncrementTextSize(const std::string &text, const float increment)
{
    std::vector<std::shared_ptr<Text>>::iterator textIt(GetPositionOfText(text));
    if(textIt != mTextVec.end())
    {
        UpdateSizeAndPosition(*(*textIt), (*textIt)->mSize + increment);
    }
}

void TextRenderer::IncrementAllTextSize(const float increment)
{
    for(auto & text : mTextVec)
        UpdateSizeAndPosition(*text, text->mSize + increment);
}

TextRenderer::Text::~Text()
{
    if(glIsVertexArray(mVao))
        glDeleteVertexArrays(1, &mVao);
    if(glIsBuffer(mVbo))
        glDeleteBuffers(1, &mVbo);
}


void TextRenderer::Text::UpdateModel()
{
    glm::mat4 translation(glm::translate(glm::mat4(1.0f),
                                         glm::vec3(mXOffset, mYOffset, 0.0f)));
    glm::mat4 scale(glm::scale(glm::mat4(1.0f), glm::vec3(mSize, mSize, 0.0f)));
    mModel = translation * scale;
}

void TextRenderer::Text::CreateVao(const std::string &text)
{
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    std::vector<glm::vec2> positions;
    CreateVbo(text, positions);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);

    std::size_t offset(sizeof(glm::vec2) * (positions.size() / 2));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offset);

    positions.clear();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextRenderer::Text::CreateVbo(const std::string &text, std::vector<glm::vec2> &posVec)
{
    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);


    float x(-(text.size() / 2.0f));
    for(std::size_t size(0); size < text.size(); ++size)
    {
        glm::vec2 bottomLeft(   x,          -0.5f);
        glm::vec2 bottomRight(  x + 1.0f,   -0.5f);
        glm::vec2 topLeft(      x,           0.5f);
        glm::vec2 topRight(     x + 1.0f,    0.5f);

        posVec.push_back(bottomLeft);
        posVec.push_back(bottomRight);
        posVec.push_back(topRight);
        posVec.push_back(bottomLeft);
        posVec.push_back(topRight);
        posVec.push_back(topLeft);

        x += 1.0f;
    }
    AddTexCoords(text, posVec);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * posVec.size(),
                 posVec.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextRenderer::Text::AddTexCoords(const std::string &text, std::vector<glm::vec2> &posVec)
{
    for(unsigned int i(0); i < text.size(); ++i)
    {
        float positionX(0.0f), positionY(0.0f);

        GetTexCoordLocation(text[i], positionX, positionY);

        glm::vec2 positionSmall(positionX * 0.0625, positionY * 0.0625);
        glm::vec2 positionBig(positionSmall.x + 0.0625, positionSmall.y + 0.0625);

        posVec.push_back(glm::vec2(positionSmall.x, positionBig.y));
        posVec.push_back(glm::vec2(positionBig.x,   positionBig.y));
        posVec.push_back(glm::vec2(positionBig.x,   positionSmall.y));
        posVec.push_back(glm::vec2(positionSmall.x, positionBig.y));
        posVec.push_back(glm::vec2(positionBig.x,   positionSmall.y));
        posVec.push_back(glm::vec2(positionSmall.x, positionSmall.y));
    }
}

void TextRenderer::Text::GetTexCoordLocation(const char &letter, float &positionX, float &positionY)
{
    switch(letter)
    {
    case ' ':
        positionX = 0.0f; positionY = 8.0f; break;
    case '!':
        positionX = 1.0f; positionY = 8.0f; break;
    case '"':
        positionX = 2.0f; positionY = 8.0f; break;
    case '#':
        positionX = 3.0f; positionY = 8.0f; break;
    case '$':
        positionX = 4.0f; positionY = 8.0f; break;
    case '%':
        positionX = 5.0f; positionY = 8.0f; break;
    case '&':
        positionX = 6.0f; positionY = 8.0f; break;
    case '\'':
        positionX = 7.0f; positionY = 8.0f; break;
    case '(':
        positionX = 8.0f; positionY = 8.0f; break;
    case ')':
        positionX = 9.0f; positionY = 8.0f; break;
    case '*':
        positionX = 10.0f; positionY = 8.0f; break;
    case '+':
        positionX = 11.0f; positionY = 8.0f; break;
    case ',':
        positionX = 12.0f; positionY = 8.0f; break;
    case '-':
        positionX = 13.0f; positionY = 8.0f; break;
    case '.':
        positionX = 14.0f; positionY = 8.0f; break;
    case '/':
        positionX = 15.0f; positionY = 8.0f; break;
    case '0':
        positionX = 0.0f; positionY = 9.0f; break;
    case '1':
        positionX = 1.0f; positionY = 9.0f; break;
    case '2':
        positionX = 2.0f; positionY = 9.0f; break;
    case '3':
        positionX = 3.0f; positionY = 9.0f; break;
    case '4':
        positionX = 4.0f; positionY = 9.0f; break;
    case '5':
        positionX = 5.0f; positionY = 9.0f; break;
    case '6':
        positionX = 6.0f; positionY = 9.0f; break;
    case '7':
        positionX = 7.0f; positionY = 9.0f; break;
    case '8':
        positionX = 8.0f; positionY = 9.0f; break;
    case '9':
        positionX = 9.0f; positionY = 9.0f; break;
    case ':':
        positionX = 10.0f; positionY = 9.0f; break;
    case ';':
        positionX = 11.0f; positionY = 9.0f; break;
    case '<':
        positionX = 12.0f; positionY = 9.0f; break;
    case '=':
        positionX = 13.0f; positionY = 9.0f; break;
    case '>':
        positionX = 14.0f; positionY = 9.0f; break;
    case '?':
        positionX = 15.0f; positionY = 9.0f; break;
    case '@':
        positionX = 0.0f; positionY = 10.0f; break;
    case 'A':
        positionX = 1.0f; positionY = 10.0f; break;
    case 'B':
        positionX = 2.0f; positionY = 10.0f; break;
    case 'C':
        positionX = 3.0f; positionY = 10.0f; break;
    case 'D':
        positionX = 4.0f; positionY = 10.0f; break;
    case 'E':
        positionX = 5.0f; positionY = 10.0f; break;
    case 'F':
        positionX = 6.0f; positionY = 10.0f; break;
    case 'G':
        positionX = 7.0f; positionY = 10.0f; break;
    case 'H':
        positionX = 8.0f; positionY = 10.0f; break;
    case 'I':
        positionX = 9.0f; positionY = 10.0f; break;
    case 'J':
        positionX = 10.0f; positionY = 10.0f; break;
    case 'K':
        positionX = 11.0f; positionY = 10.0f; break;
    case 'L':
        positionX = 12.0f; positionY = 10.0f; break;
    case 'M':
        positionX = 13.0f; positionY = 10.0f; break;
    case 'N':
        positionX = 14.0f; positionY = 10.0f; break;
    case 'O':
        positionX = 15.0f; positionY = 10.0f; break;
    case 'P':
        positionX = 0.0f; positionY = 11.0f; break;
    case 'Q':
        positionX = 1.0f; positionY = 11.0f; break;
    case 'R':
        positionX = 2.0f; positionY = 11.0f; break;
    case 'S':
        positionX = 3.0f; positionY = 11.0f; break;
    case 'T':
        positionX = 4.0f; positionY = 11.0f; break;
    case 'U':
        positionX = 5.0f; positionY = 11.0f; break;
    case 'V':
        positionX = 6.0f; positionY = 11.0f; break;
    case 'W':
        positionX = 7.0f; positionY = 11.0f; break;
    case 'X':
        positionX = 8.0f; positionY = 11.0f; break;
    case 'Y':
        positionX = 9.0f; positionY = 11.0f; break;
    case 'Z':
        positionX = 10.0f; positionY = 11.0f; break;
    case '[':
        positionX = 11.0f; positionY = 11.0f; break;
    case '\\':
        positionX = 12.0f; positionY = 11.0f; break;
    case ']':
        positionX = 13.0f; positionY = 11.0f; break;
    case '^':
        positionX = 14.0f; positionY = 11.0f; break;
    case '_':
        positionX = 15.0f; positionY = 11.0f; break;
    case '`':
        positionX = 0.0f; positionY = 12.0f; break;
    case 'a':
        positionX = 1.0f; positionY = 12.0f; break;
    case 'b':
        positionX = 2.0f; positionY = 12.0f; break;
    case 'c':
        positionX = 3.0f; positionY = 12.0f; break;
    case 'd':
        positionX = 4.0f; positionY = 12.0f; break;
    case 'e':
        positionX = 5.0f; positionY = 12.0f; break;
    case 'f':
        positionX = 6.0f; positionY = 12.0f; break;
    case 'g':
        positionX = 7.0f; positionY = 12.0f; break;
    case 'h':
        positionX = 8.0f; positionY = 12.0f; break;
    case 'i':
        positionX = 9.0f; positionY = 12.0f; break;
    case 'j':
        positionX = 10.0f; positionY = 12.0f; break;
    case 'k':
        positionX = 11.0f; positionY = 12.0f; break;
    case 'l':
        positionX = 12.0f; positionY = 12.0f; break;
    case 'm':
        positionX = 13.0f; positionY = 12.0f; break;
    case 'n':
        positionX = 14.0f; positionY = 12.0f; break;
    case 'o':
        positionX = 15.0f; positionY = 12.0f; break;
    case 'p':
        positionX = 0.0f; positionY = 13.0f; break;
    case 'q':
        positionX = 1.0f; positionY = 13.0f; break;
    case 'r':
        positionX = 2.0f; positionY = 13.0f; break;
    case 's':
        positionX = 3.0f; positionY = 13.0f; break;
    case 't':
        positionX = 4.0f; positionY = 13.0f; break;
    case 'u':
        positionX = 5.0f; positionY = 13.0f; break;
    case 'v':
        positionX = 6.0f; positionY = 13.0f; break;
    case 'w':
        positionX = 7.0f; positionY = 13.0f; break;
    case 'x':
        positionX = 8.0f; positionY = 13.0f; break;
    case 'y':
        positionX = 9.0f; positionY = 13.0f; break;
    case 'z':
        positionX = 10.0f; positionY = 13.0f; break;
    case '{':
        positionX = 11.0f; positionY = 13.0f; break;
    case '|':
        positionX = 12.0f; positionY = 13.0f; break;
    case '}':
        positionX = 13.0f; positionY = 13.0f; break;
    case '~':
        positionX = 14.0f; positionY = 13.0f; break;
    default:
        positionX = 0.0f; positionY = 0.0f; break;
    }
}

std::vector<std::shared_ptr<TextRenderer::Text>>::iterator
    TextRenderer::GetPositionOfText(const std::string &text)
{
    auto it(mTextVec.begin());
    for(; it != mTextVec.end(); ++it)
    {
        if((*it)->mText == text)
            break;
    }
    return it;
}

float TextRenderer::AlignVertically(const Alignment alignment, const float size)
{
    float halfSize(size * 0.5);
    float yOffset(0.0f);
    switch(alignment)
    {
    case Alignment::Top:
        yOffset = mHeight - halfSize;
        break;
    case Alignment::Center:
        yOffset = mHeight * 0.5f;
        break;
    case Alignment::Bottom:
        yOffset = halfSize;
        break;
    case Alignment::Right:
    case Alignment::Left:
    default:
        std::cerr << "Sorry wrong value passed to alignment vertical\n";
        break;
    }
    return yOffset;
}

float TextRenderer::AlignHorizontally(const Alignment alignment, const size_t stringLength,
                                      const float size)
{
    float xOffset(0.0f);
    float halfSize((stringLength * size) * 0.5f);
    switch (alignment)
    {
    case Alignment::Left:
        xOffset = halfSize;
        break;
    case Alignment::Center:
        xOffset = mWidth * 0.5f;
        break;
    case Alignment::Right:
        xOffset = mWidth - halfSize;
        break;
    case Alignment::Top:
    case Alignment::Bottom:
    default:
        std::cerr << "Sorry wrong value passed to alignment horizontal\n";
        break;

    }
    return xOffset;
}

float TextRenderer::GetSizeOfAllText(const std::vector<std::string> &textVec)
{
    float size(0.0f);
    for(auto & text : textVec)
    {
        size += text.size() * 1.0f;
    }
    return size;
}

float TextRenderer::GetOffsetForCenterVec(const std::vector<std::string> &textVec, float size)
{
    auto text(textVec.begin());

    float offset(-((GetSizeOfAllText(textVec) * 0.5f) * size));
    offset += ((text->size() * 0.5f) * size);

    return offset;
}

bool TextRenderer::IsTextInVector(const std::string &text)
{
    bool found(false);
    if(!mTextVec.empty())
    {
        for(auto it(mTextVec.begin()); it != mTextVec.end() && !found; ++it)
        {
            if((*it)->mText == text)
                found = true;
        }
    }
    return found;
}

void TextRenderer::Draw(const Text &text)
{
    glBindVertexArray(text.mVao);
    glUniformMatrix4fv(glGetUniformLocation(mProgram, "view"), 1, GL_FALSE,
                       glm::value_ptr(mView * text.mModel));
    glUniform4fv(glGetUniformLocation(mProgram, "colour"), 1, glm::value_ptr(text.mColour));
    glDrawArrays(GL_TRIANGLES, 0, text.mText.size() * 6);
    glBindVertexArray(0);
}

void TextRenderer::UpdateSizeAndPosition(Text &text, const float size)
{
    {
        float oldXOffset(text.mXOffset);
        float oldTextX((text.mText.size() * text.mSize) * 0.5f);

        if(oldXOffset - oldTextX <= 0 || oldXOffset + oldTextX >= mWidth)
        {
            text.mXOffset = (oldXOffset - oldTextX <= 0.0f ?
                                 AlignHorizontally(Alignment::Left, text.mText.size(), size) :
                                 AlignHorizontally(Alignment::Right, text.mText.size(), size));
        }
        else if(oldXOffset == (mWidth * 0.5f))
        {
            text.mXOffset = AlignHorizontally(Alignment::Center, text.mText.size(), size);
        }
        else
        {
            text.mXOffset = (oldXOffset - oldTextX) + ((text.mText.size() * size) * 0.5f);
        }
    }

    float oldYOffset(text.mYOffset);
    float oldTextY(text.mSize * 0.5);
    if(oldYOffset - oldTextY <= 0 || oldYOffset + oldTextY >= mHeight)
    {
        text.mYOffset = (oldYOffset - oldTextY <= 0.0f ?
                             AlignVertically(Alignment::Bottom, size) :
                AlignVertically(Alignment::Top, size));
    }
    else if(oldYOffset == (mHeight * 0.5f))
    {
        text.mYOffset = AlignVertically(Alignment::Center, size);
    }
    else
    {
        text.mYOffset = (oldYOffset - oldTextY) + (size * 0.5f);
    }
    text.mSize = size;
    text.UpdateModel();
}
