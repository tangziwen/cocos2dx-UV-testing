#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void cylinderUpdate(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    cocos2d::GLProgramState * state;
    float cylinder_texture_offset;
    float shining_duraion;
};

#endif // __HELLOWORLD_SCENE_H__
