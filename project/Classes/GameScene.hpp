//
//  GameScene.hpp
//  PulseCatch-desktop
//
//  Created by hoangbui-fe on 11/8/26.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"

class GameScene : public cocos2d::Scene {
public:
    GameScene();
    ~GameScene();
    
public:
    enum ChildTag {
        None = 0,
        kChildTagBackground,
        kChildTagRing,
        kChildTagTarget,
        kChildTagHUD,
        kChildTagHUDGameOver,
    };
    
public:
    static cocos2d::Scene* createScene();

private:
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    virtual void update(float deltaTime) override;
    virtual void onEnter() override;
    virtual void onExit() override;

private:
    cocos2d::DrawNode* _background = nullptr;
    cocos2d::DrawNode* _ring = nullptr;
    cocos2d::DrawNode* _target = nullptr;

    cocos2d::Label* _livesLabel = nullptr;
    cocos2d::Label* _scoreLabel = nullptr;
    cocos2d::Label* _gameOverLabel = nullptr;
    cocos2d::Label* _finalScoreLabel = nullptr;
    cocos2d::Label* _bestScoreLabel = nullptr;

    cocos2d::Vec2 _targetPosition = cocos2d::Vec2::ZERO;

    float _ringRadius = 0.0f;
    float _targetLifetime = 0.0f;

    int _lives = 0;
    int _score = 0;

    bool _playing = false;

    int _sessionBestScore = 0;
    
private:
    void createVisuals();
    void createInputListeners();

    void startNewGame();
    void spawnTarget();
    void clearTarget();

    void updateRing(float deltaTime);
    void updateTargetLifetime(float deltaTime);
    void updateHUD();

    void handlePointer(const cocos2d::Vec2& screenPosition);
    void loseLife();
    void enterGameOver();

    float getCurrentRingSpeed() const;
    float getCurrentVisibilityWindow() const;
};

#endif /* GameScene_hpp */
