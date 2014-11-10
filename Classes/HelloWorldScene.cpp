#include "HelloWorldScene.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    //the offset use to translating texture
    this->cylinder_texture_offset = 0;
    this->shining_duraion = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();

    //use custom camera
    auto camera = Camera::createPerspective(60,visibleSize.width/visibleSize.height,0.1,200);
    camera->setCameraFlag(CameraFlag::USER1);
 
    auto cylinder = Sprite3D::create("cylinder.c3b");
    auto shader =GLProgram::createWithFilenames("cylinder.vert","cylinder.frag");
    
    state = GLProgramState::create(shader);

    cylinder->setGLProgramState(state);
    
    //set attrib to shader
    long offset = 0;
    auto attributeCount = cylinder->getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = cylinder->getMesh()->getMeshVertexAttribute(k);
        state->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
            meshattribute.size,
            meshattribute.type,
            GL_FALSE,
            cylinder->getMesh()->getVertexSizeInBytes(),
            (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
    
    auto shining_texture = Director::getInstance()->getTextureCache()->addImage("caustics.png");
    Texture2D::TexParams tRepeatParams;
    tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
    tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    shining_texture->setTexParameters(tRepeatParams);
    state->setUniformTexture("caustics",shining_texture);
    cylinder->setPosition3D(Vec3(0,-15,-50));
    cylinder->setRotation3D(Vec3(-90,0,0));
    this->addChild(cylinder);
    this->setCameraMask(2);
    this->addChild(camera);

    schedule(schedule_selector(HelloWorld::cylinderUpdate));
    return true;
}


void HelloWorld::cylinderUpdate(float dt)
{
    static bool fade_in = true;
    cylinder_texture_offset += 0.3*dt;
    cylinder_texture_offset = (cylinder_texture_offset >1)?0:cylinder_texture_offset;
    if(fade_in)
    {
        shining_duraion += 0.5*dt;
        if(shining_duraion>1) fade_in = false; 
    }
    else
    {
        shining_duraion -= 0.5*dt;
        if(shining_duraion<0) fade_in = true;
    }
    state->setUniformFloat("offset",cylinder_texture_offset);
    state->setUniformFloat("duration",shining_duraion);
}
