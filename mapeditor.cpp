#define GLM_FORCE_RADIANS

#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "textrenderer.hpp"
#include "map.hpp"
#include "mapeditor.hpp"
#include "createbuffer.hpp"
#include "resourcemanager.hpp"
#include "visibleobject.hpp"
#include "menuupdate.hpp"
#include "writedata.hpp"
#include "ai.hpp"
#include "setupobjects.hpp"
#include "character.hpp"

struct PatrolArea
{
    PatrolArea(const unsigned int playerIn, const float xIn,
               const float yIn, const float x1In, const float y1In)
        :player(playerIn), x(xIn), y(yIn), x1(x1In), y1(y1In){}
    unsigned int player;
    float x;
    float y;
    float x1;
    float y1;
};

bool Menu(std::vector<std::shared_ptr<VisibleObject>> &mapVec,
          ResourceManager &resMan, std::vector<PatrolArea> &patVec);
void SaveMap(std::vector<std::shared_ptr<VisibleObject>> &mapVec,
             ResourceManager &resMan, std::vector<PatrolArea> &patVec);
void LoadMap(std::vector<std::shared_ptr<VisibleObject>> &mapVec,
             ResourceManager &resMan, std::vector<PatrolArea> &patVec);
void SetupMap(std::vector<std::shared_ptr<VisibleObject>> &mapVec,
              ResourceManager &resMan, const char * filename, std::vector<PatrolArea> &patVec);
void SelectObject(const int x, const int y, unsigned int &enemy,
                  const std::shared_ptr<unsigned int> &geoffTexture,
                  const std::shared_ptr<VisibleObject> &positionPtr,
                  std::shared_ptr<VisibleObject> &selectedObject,
                  const std::vector<std::shared_ptr<VisibleObject>> &objVec,
                  std::vector<std::shared_ptr<VisibleObject>> &mapVec,
                  glm::vec2 &position, std::vector<PatrolArea> &patVec);
void UpdateSelectedObjectPos(std::shared_ptr<VisibleObject> &selectedObject);
void PutPatrolCoordsToVec(std::vector<float> &mMap, const std::vector<PatrolArea> &patVec, const std::vector<std::shared_ptr<VisibleObject>> &mapVec);
void RemoveFromVector(std::vector<std::shared_ptr<VisibleObject> > &vect,
                      const std::shared_ptr<VisibleObject> &obj, std::vector<PatrolArea> &patVec);
void GetMousePos(int &x, int &y);
void CreateResources(ResourceManager &resManager);
void FillObjectVector(std::vector<std::shared_ptr<VisibleObject>> &objVec,
                      ResourceManager &resManager);
void PutObjectInVector(const int x, const int y,
                       std::shared_ptr<VisibleObject> &selectedObject,
                       std::vector<std::shared_ptr<VisibleObject>> &objVec);
bool Input();
bool GetMouseButton(std::shared_ptr<VisibleObject> &obj, bool &toRemove,
                    const std::shared_ptr<VisibleObject> &positionPtr);
bool CheckVectorAndRemove(std::vector<std::shared_ptr<VisibleObject>> &vect,
                          const std::shared_ptr<VisibleObject> &object,
                          const std::shared_ptr<unsigned int> geoffTexture);
bool LowestPos(std::shared_ptr<VisibleObject> &a, std::shared_ptr<VisibleObject> &b);
std::shared_ptr<VisibleObject> GetObjectFromVector(const int x, const int y, const unsigned int &enemy,
        const std::vector<std::shared_ptr<VisibleObject>> &objVec);

void MakeMap()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 view(glm::ortho(0.0f, 40.0f, 0.0f, 30.0f, 1.0f, -1.0f));

    ResourceManager resMan;
    CreateResources(resMan);
    std::shared_ptr<unsigned int> geoffTexture(resMan.GetTexture("geoff.png"));
    std::shared_ptr<VisibleObject> posPtr(new VisibleObject
                                          (0.0f, 0.0f, resMan.GetVao("vao top left"),
                                           nullptr, resMan.GetTexture("scenery2.png"),
                                           resMan.GetProgram("textured.vs", "textured.fs")));
    std::vector<std::shared_ptr<VisibleObject> > objVec;
    std::vector<std::shared_ptr<VisibleObject> > mapVec;
    std::vector<PatrolArea> patVec;

    FillObjectVector(objVec, resMan);

    bool making(true);
    std::shared_ptr<VisibleObject> selectedObject(nullptr);
    unsigned int enemy(1);
    glm::vec2 pos1(-1.0f, -1.0f);
    glm::vec2 pos2(-1.0f, -1.0f);

    while(making && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        int x, y;
        GetMousePos(x, y);
        UpdateSelectedObjectPos(selectedObject);
        SelectObject(x, y, enemy, geoffTexture, posPtr, selectedObject, objVec, mapVec, (pos1.x == -1.0f ? pos1 : pos2), patVec);

        if(pos2.x != -1.0f)
        {
            PatrolArea patArea(enemy-1, pos1.x, pos1.y, pos2.x, pos2.y);
            patVec.push_back(patArea);
            pos1.x = -1.0f;
            pos1.y = -1.0f;
            pos2.x = -1.0f;
            pos2.y = -1.0f;
            selectedObject = nullptr;
        }

        for(auto & obj : objVec)
        {
            if(obj->GetTexture() == 0)
            {
                if((int)obj->GetX() == x && (int)obj->GetY() == y)
                    obj->Draw(view, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
                else
                    obj->Draw(view, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            }
            else
                obj->Draw(view);
        }
        if(selectedObject)
            selectedObject->Draw(view);
        for(auto & obj : mapVec)
        {
            obj->Draw(view);
        }

        if(Input())
        {
            if(!Menu(mapVec, resMan, patVec))
                making = false;
        }

        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }
}

void SelectObject(const int x, const int y, unsigned int &enemy,
                  const std::shared_ptr<unsigned int> &geoffTexture,
                  const std::shared_ptr<VisibleObject> &positionPtr,
                  std::shared_ptr<VisibleObject> &selected,
                  const std::vector<std::shared_ptr<VisibleObject> > &objVec,
                  std::vector<std::shared_ptr<VisibleObject> > &mapVec,
                  glm::vec2 &pos, std::vector<PatrolArea> &patVec)
{
    bool mouseClicked(false);
    bool toRemove(false);

    mouseClicked = GetMouseButton(selected, toRemove, positionPtr);

    if(mouseClicked && !selected)
        selected = GetObjectFromVector(x, y, enemy, objVec);
    else if(mouseClicked && selected && x < 28 && y < 30)
    {
        if(selected == positionPtr)
        {
            pos.x = (int)selected->GetX();
            pos.y = (int)selected->GetY();
        }
        else if(!CheckVectorAndRemove(mapVec, selected, geoffTexture))
        {
            if(selected->GetPlayer() == 0)
            {
                std::shared_ptr<VisibleObject> ptr(
                            new VisibleObject((int)selected->GetX() + 0.5f, (int)selected->GetY() + 0.5f,
                                              selected->GetVao(), selected->GetVao(),
                                              selected->GetTexture(), selected->GetProgram()));
                mapVec.push_back(ptr);
            }
            else
            {
                std::shared_ptr<VisibleObject> ptr(
                            new Character((int)selected->GetX() + 0.5f, (int)selected->GetY() + 0.5f, 4.0f,
                            enemy, selected->GetVao(), selected->GetVao(),
                            selected->GetTexture(), selected->GetProgram()));
                enemy += 1;
                mapVec.push_back(ptr);
                selected = positionPtr;
            }
        }
        mouseClicked = false;
    }

    if(toRemove && x < 28 && y < 30)
    {
        selected = GetObjectFromVector(x, y, enemy, mapVec);
        if(selected)
        {
            RemoveFromVector(mapVec, selected, patVec);
            selected = nullptr;
        }
    }

}

bool GetMouseButton(std::shared_ptr<VisibleObject> &obj,
                    bool &toRemove, const std::shared_ptr<VisibleObject> &posPtr)
{
    static bool mouseClickedAlready(false);
    bool retValue(false);

    if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT)
            == GLFW_PRESS && !mouseClickedAlready)
    {
        mouseClickedAlready = true;
        retValue = true;
    }
    else if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT)
            == GLFW_RELEASE && mouseClickedAlready)
    {
        mouseClickedAlready = false;
    }

    if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT)
            == GLFW_PRESS)
    {
        if(obj)
        {
            if(obj != posPtr)
            {
                obj = nullptr;
            }
        }
        else
        {
            toRemove = true;
        }
    }
    return retValue;
}

std::shared_ptr<VisibleObject> GetObjectFromVector(const int x, const int y, const unsigned int &enemy,
                                                   const std::vector<std::shared_ptr<VisibleObject> > &objVec)
{
    std::shared_ptr<VisibleObject> ptr(nullptr);
    for(auto & obj : objVec)
    {
        if((int)obj->GetX() == x && (int)obj->GetY() == y && obj->GetTexture() > 0)
        {
            if(obj->GetPlayer() < 2)
            {
                ptr = std::make_shared<VisibleObject>(x + 0.5f, y + 0.5f, obj->GetVao(),
                                                      obj->GetVao(), obj->GetTexture(),
                                                      obj->GetProgram());
            }
            else
            {
                ptr = std::make_shared<Character>(x + 0.5f, y + 0.5f, 4.0f,
                                                  enemy, obj->GetVao(), obj->GetVao(),
                                                  obj->GetTexture(), obj->GetProgram());
            }
            break;
        }
    }
    return ptr;
}

bool CheckVectorAndRemove(std::vector<std::shared_ptr<VisibleObject> > &vect,
                          const std::shared_ptr<VisibleObject> &object,
                          const std::shared_ptr<unsigned int> geoffTexture)
{
    bool retValue(false);

    for(auto obj(vect.begin()); obj != vect.end();)
    {
        if((*obj)->GetTexture() == geoffTexture)
            obj = vect.erase(obj); // so only one geoff can exist
        else if((int)(*obj)->GetX() == (int)object->GetX() &&
                (int)(*obj)->GetY() == (int)object->GetY())
        {

            if((*obj)->GetVao() == object->GetVao() &&
                    (*obj)->GetTexture() == object->GetTexture())
                retValue = true;
            else
                vect.erase(obj);
            ++obj;
        }
        else
            ++obj;
    }
    return retValue;
}

void RemoveFromVector(std::vector<std::shared_ptr<VisibleObject> > &vect,
                      const std::shared_ptr<VisibleObject> &object, std::vector<PatrolArea> &patVec)
{
    for(auto obj(vect.begin()); obj != vect.end(); ++obj)
    {
        if((int)(*obj)->GetX() == (int)object->GetX() &&
                (int)(*obj)->GetY() == (int)object->GetY())
        {
            if((*obj)->GetPlayer() > 1)
            {
                unsigned int player((*obj)->GetPlayer());
                for(auto patrol(patVec.begin()); patrol != patVec.end(); ++patrol)
                {
                    if((*patrol).player == player)
                        patVec.erase(patrol);
                    break;
                }
            }
            vect.erase(obj);
            break;
        }
    }
}

bool Menu(std::vector<std::shared_ptr<VisibleObject> > &mapVec,
          ResourceManager &resMan, std::vector<PatrolArea> &patVec)
{
    bool retValue(true);
    std::vector<std::string> textVec = {"Continue", "Load", "Save", "Quit"};
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    bool playing(true);
    TextRenderer textRenderer(resMan.GetTexture("text.png"));
    textRenderer.AddTextVerticalAlign(textVec, TextRenderer::Alignment::Center,
                                      TextRenderer::Alignment::Center, 30,
                                      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    textRenderer.ChangeTextColour("Continue", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    unsigned int menuPos(0);
    bool enterPressed(false);

    while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        menuPos = UpdateList(textRenderer, menuPos, textVec);
        if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS &&
                !enterPressed)
        {
            enterPressed = true;
        }
        else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER)
                == GLFW_RELEASE && enterPressed)
        {
            playing = false;
            enterPressed = false;
        }

        textRenderer.DrawAll();
        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }

    if(!playing)
    {
        if(menuPos == 1)
        {
            LoadMap(mapVec, resMan, patVec);
        }
        else if(menuPos == 2)
        {
            SaveMap(mapVec, resMan, patVec);
        }
        else if(menuPos == 3)
        {
            retValue = false;
        }
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
    return retValue;
}

void SaveMap(std::vector<std::shared_ptr<VisibleObject> > &mapVec,
             ResourceManager &resMan, std::vector<PatrolArea> &patVec)
{
    std::array<unsigned int, 840> mMap;
    mMap.fill(0);
    std::vector<float> mAIMap;

    std::sort(mapVec.begin(), mapVec.end(), LowestPos);
    for(auto & obj : mapVec)
    {
        int where((int)obj->GetX() + ((int)obj->GetY() * 28));
        int what = 0;
        if(obj->GetTexture() == resMan.GetTexture("geoff.png"))
        {
            what = 9;
        }
        else if(obj->GetTexture() == resMan.GetTexture("wolf.png"))
        {
            what = 10;
        }
        else if(obj->GetTexture() == resMan.GetTexture("bear.png"))
        {
            what = 11;
        }
        else if(obj->GetTexture() == resMan.GetTexture("scenery.png"))
        {
            if(obj->GetVao() == resMan.GetVao("vao top left"))
            {
                what = 1;
            }
            else if(obj->GetVao() == resMan.GetVao("vao top right"))
            {
                what = 3;
            }
            else if(obj->GetVao() == resMan.GetVao("vao bottom right"))
            {
                what = 4;
            }
        }
        else if(obj->GetTexture() == resMan.GetTexture("scenery2.png"))
        {
            if(obj->GetVao() == resMan.GetVao("vao top right"))
            {
                what = 7;
            }
            else if(obj->GetVao() == resMan.GetVao("vao bottom left"))
            {
                what = 5;
            }
            else if(obj->GetVao() == resMan.GetVao("vao bottom right"))
            {
                what = 8;
            }
        }
        mMap[where] = what;
    }
    PutPatrolCoordsToVec(mAIMap, patVec, mapVec);
    int i(0);
    for(bool mapSaved(false); !mapSaved; ++i)
    {
        std::string map = "map" + std::to_string(i) + ".txt";
        if(WriteData(mAIMap, mMap, map.c_str()))
            mapSaved = true;
    }
}

void LoadMap(std::vector<std::shared_ptr<VisibleObject> > &mapVec,
             ResourceManager &resMan, std::vector<PatrolArea> &patVec)
{
    int i(0);
    std::vector<std::string> textVec;
    for(bool exists(true); exists; ++i)
    {
        std::string map = "map" + std::to_string(i) + ".txt";
        if(!FileExists(map.c_str()))
            exists = false;
        else
            textVec.push_back(map);
    }
    textVec.push_back("Exit");
    TextRenderer textRenderer(resMan.GetTexture("text.png"));
    textRenderer.AddTextVerticalAlign(textVec, TextRenderer::Alignment::Center,
                                      TextRenderer::Alignment::Center, 30,
                                      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    textRenderer.ChangeTextColour(*textVec.begin(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    bool playing(true), enterPressed(false);
    unsigned int menuPos(0);

    while(playing && !glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        menuPos = UpdateList(textRenderer, menuPos, textVec);

        if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS &&
                !enterPressed)
        {
            enterPressed = true;
        }
        else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER)
                == GLFW_RELEASE && enterPressed)
        {
            playing = false;
            enterPressed = false;
        }

        textRenderer.DrawAll();

        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }
    if(!playing)
    {
        if(menuPos != textVec.size() - 1)
        {
            mapVec.clear();
            patVec.clear();
            SetupMap(mapVec, resMan, (*(textVec.begin() + menuPos)).c_str(), patVec);
        }
    }
}

void SetupMap(std::vector<std::shared_ptr<VisibleObject> > &mapVec,
              ResourceManager &resMan, const char *filename, std::vector<PatrolArea> &patVec)
{
    Map map(filename);
    std::vector<AI> aiVec;
    GetVecFullOfObjects(mapVec, map, resMan, aiVec);
    for(auto & ai : aiVec)
    {
        PatrolArea patArea(ai.GetPlayer(), ai.mFirstPos.x, ai.mFirstPos.y, ai.mSecondPos.x, ai.mSecondPos.y);
        patVec.push_back(patArea);
    }
}

void UpdateSelectedObjectPos(std::shared_ptr<VisibleObject> &selectedObject)
{
    if(selectedObject)
    {
        double mouseX, mouseY;
        int iWindowX, iWindowY;

        glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
        glfwGetWindowSize(glfwGetCurrentContext(), &iWindowX, &iWindowY);
        mouseX /= ((double)iWindowX / 40.0);
        mouseY = 30.0 - (mouseY / ((double)iWindowY / 30.0));

        selectedObject->SetX(mouseX);
        selectedObject->SetY(mouseY);
    }
}

void GetMousePos(int &x, int &y)
{

    double mouseX, mouseY;
    int iWindowX, iWindowY;

    glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
    glfwGetWindowSize(glfwGetCurrentContext(), &iWindowX, &iWindowY);

    double screenPosX((double)iWindowX / 40.0),
            screenPosY((double)iWindowY / 30.0);

    x = (int)(mouseX / screenPosX);
    y = (int)(30 - (mouseY / screenPosY));
}

void CreateResources(ResourceManager &resourceManager)
{
    resourceManager.CreateTexture("geoff.png");
    resourceManager.CreateTexture("wolf.png");
    resourceManager.CreateTexture("bear.png");
    resourceManager.CreateTexture("scenery.png");
    resourceManager.CreateTexture("scenery2.png");
    resourceManager.CreateTexture("text.png");

    resourceManager.CreateProgram("textured.vs", "textured.fs");
    resourceManager.CreateProgram("character.vs", "textured.fs");
    resourceManager.CreateProgram("colouredsquare.vs", "colouredsquare.fs");

    std::shared_ptr<unsigned int> buffer(new unsigned int (CreateBuffer())),
    vao(new unsigned int (CreateVAO(*buffer, 1))),
    vaoTopLeft(new unsigned int (CreateVAO(*buffer, 2))),
    vaoTopRight(new unsigned int (CreateVAO(*buffer, 3))),
    vaoBottomLeft(new unsigned int (CreateVAO(*buffer, 4))),
    vaoBottomRight(new unsigned int (CreateVAO(*buffer, 5))),
    vaoPlainColour(new unsigned int (SetupStandardVAO(*buffer)));

    resourceManager.AddVao("buffer", buffer);
    resourceManager.AddVao("full vao", vao);
    resourceManager.AddVao("vao top left", vaoTopLeft);
    resourceManager.AddVao("vao top right", vaoTopRight);
    resourceManager.AddVao("vao bottom left", vaoBottomLeft);
    resourceManager.AddVao("vao bottom right", vaoBottomRight);
    resourceManager.AddVao("vao plain colour", vaoPlainColour);
}

void FillObjectVector(std::vector<std::shared_ptr<VisibleObject> > &objVec,
                      ResourceManager &resManager)
{
    std::shared_ptr<unsigned int> vaoPlainColour(resManager.GetVao("vao plain colour"));
    std::shared_ptr<unsigned int> vaoTopLeft(resManager.GetVao("vao top left"));
    std::shared_ptr<unsigned int> vaoTopRight(resManager.GetVao("vao top right"));
    std::shared_ptr<unsigned int> vaoBottomLeft(resManager.GetVao("vao bottom left"));
    std::shared_ptr<unsigned int> vaoBottomRight(resManager.GetVao("vao bottom right"));

    std::shared_ptr<unsigned int> programPlainColour(resManager.GetProgram("colouredsquare.vs", "colouredsquare.fs"));
    std::shared_ptr<unsigned int> programTextured(resManager.GetProgram("textured.vs", "textured.fs"));
    std::shared_ptr<unsigned int> programCharacter(resManager.GetProgram("character.vs", "textured.fs"));

    std::shared_ptr<unsigned int> geoffTex(resManager.GetTexture("geoff.png"));
    std::shared_ptr<unsigned int> wolfTex(resManager.GetTexture("wolf.png"));
    std::shared_ptr<unsigned int> bearTex(resManager.GetTexture("bear.png"));
    std::shared_ptr<unsigned int> sceneryTex(resManager.GetTexture("scenery.png"));
    std::shared_ptr<unsigned int> sceneryTex2(resManager.GetTexture("scenery2.png"));


    for(std::size_t y(0); y < 30; ++y)
    {
        for(std::size_t x(0); x < 28; ++x)
        {
            std::shared_ptr<VisibleObject> ptr(
                        new VisibleObject(0.5f + (x * 1.0f), 0.5f + (y * 1.0f), vaoPlainColour,
                                          vaoPlainColour, 0, programPlainColour));
            objVec.push_back(ptr);
        }
    }

    float x(34.5f);
    std::shared_ptr<VisibleObject> ptr(
                new VisibleObject(x, 24.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour));
    objVec.push_back(ptr);
    ptr = std::make_shared<VisibleObject>(x, 24.5f, vaoTopLeft, vaoTopLeft, geoffTex, programTextured);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x, 22.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<Character>(x, 22.5f, 4.0f, 1, vaoTopLeft, vaoTopLeft, wolfTex, programCharacter);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x, 21.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<Character>(x, 21.5f, 4.0f, 1, vaoTopLeft, vaoTopLeft, bearTex, programCharacter);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x, 19.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<VisibleObject>(x, 19.5f, vaoTopLeft, vaoTopLeft, sceneryTex, programTextured);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x, 18.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<VisibleObject>(x, 18.5f, vaoTopRight, vaoTopRight, sceneryTex, programTextured);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x + 1.0f, 19.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<VisibleObject>(x + 1.0f, 19.5f, vaoBottomRight, vaoBottomRight, sceneryTex, programTextured);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x + 1.0f, 18.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<VisibleObject>(x + 1.0f, 18.5f, vaoBottomLeft, vaoBottomLeft, sceneryTex2, programTextured);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x, 16.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<VisibleObject>(x, 16.5f, vaoTopRight, vaoTopRight, sceneryTex2, programTextured);
    objVec.push_back(ptr);

    ptr = std::make_shared<VisibleObject>(x, 15.5f, vaoPlainColour, vaoPlainColour, nullptr, programPlainColour);
    objVec.push_back(ptr);
    ptr = std::make_shared<VisibleObject>(x, 15.5f, vaoBottomRight, vaoBottomRight, sceneryTex2, programTextured);
    objVec.push_back(ptr);
}

void PutPatrolCoordsToVec(std::vector<float> &mMap, const std::vector<PatrolArea> &patVec,
                          const std::vector<std::shared_ptr<VisibleObject> > &mapVec)
{
    for(auto & obj : mapVec)
    {
        if(obj->GetPlayer() > 0)
        {
             for(auto & patrol : patVec)
            {
                if(obj->GetPlayer() == patrol.player)
                {
                    mMap.push_back(patrol.x);
                    mMap.push_back(patrol.y);
                    mMap.push_back(patrol.x1);
                    mMap.push_back(patrol.y1);
                    break;
                }
            }
        }
    }
}

bool LowestPos(std::shared_ptr<VisibleObject> &a, std::shared_ptr<VisibleObject> &b)
{
    unsigned int one((int)a->GetX() + ((int)a->GetY() * 28));
    unsigned int two((int)b->GetX() + ((int)b->GetY() * 28));
    if(one < two)
        return true;
    return false;
}

bool Input()
{
    bool retValue(false);
    static bool pPressed(false);

    if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_P) == GLFW_PRESS && !pPressed)
    {
        pPressed = true;
    }
    else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_P) == GLFW_RELEASE && pPressed)
    {
        pPressed = false;
        retValue = true;
    }
    return retValue;
}
