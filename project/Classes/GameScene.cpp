//
//  GameScene.cpp
//  PulseCatch-desktop
//
//  Created by hoangbui-fe on 11/8/26.
//

#include "GameScene.hpp"
#include "Constants.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <string>

USING_NS_CC;

GameScene::GameScene() {
    
}

GameScene::~GameScene() {
    
}

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    createVisuals();
    createInputListeners();

    return true;
}

void GameScene::onEnter() {
    Scene::onEnter();
    scheduleUpdate();
    startNewGame();
}

void GameScene::onExit() {
    unscheduleUpdate();
    Scene::onExit();
}

void GameScene::createVisuals()
{
    _background = DrawNode::create();

    _background->drawSolidRect(
        Vec2::ZERO,
        Vec2(
            PulseCatch::DESIGN_WIDTH,
            PulseCatch::DESIGN_HEIGHT
        ),
        PulseCatch::BACKGROUND_COLOR
    );

    addChild(_background, kChildTagBackground);

    _ring = DrawNode::create();
    addChild(_ring, kChildTagRing);

    _target = DrawNode::create();
    addChild(_target, kChildTagTarget);

    _livesLabel = Label::createWithTTF(
        "Lives: 3",
        "fonts/arial.ttf",
        PulseCatch::HUD_FONT_SIZE
    );

    _livesLabel->setAnchorPoint(Vec2(0.0f, 1.0f));

    _livesLabel->setPosition(
        PulseCatch::HUD_MARGIN,
        PulseCatch::DESIGN_HEIGHT -
            PulseCatch::HUD_MARGIN
    );

    _livesLabel->setTextColor(Color4B::WHITE);

    addChild(_livesLabel, kChildTagHUD);

    _scoreLabel = Label::createWithTTF(
        "Score: 0",
        "fonts/arial.ttf",
        PulseCatch::HUD_FONT_SIZE
    );

    _scoreLabel->setAnchorPoint(Vec2(1.0f, 1.0f));

    _scoreLabel->setPosition(
        PulseCatch::DESIGN_WIDTH -
            PulseCatch::HUD_MARGIN,
        PulseCatch::DESIGN_HEIGHT -
            PulseCatch::HUD_MARGIN
    );

    _scoreLabel->setTextColor(Color4B::WHITE);

    addChild(_scoreLabel, kChildTagHUD);

    _gameOverLabel = Label::createWithTTF(
        "GAME OVER",
        "fonts/arial.ttf",
        PulseCatch::GAME_OVER_FONT_SIZE
    );

    _gameOverLabel->setAnchorPoint(
        Vec2(0.5f, 0.5f)
    );

    _gameOverLabel->setPosition(
        PulseCatch::CENTER_X,
        370.0f
    );

    _gameOverLabel->setTextColor(Color4B::WHITE);
    _gameOverLabel->setVisible(false);

    addChild(_gameOverLabel, kChildTagHUDGameOver);

    _finalScoreLabel = Label::createWithTTF(
        "Score: 0",
        "fonts/arial.ttf",
        PulseCatch::RESULT_FONT_SIZE
    );

    _finalScoreLabel->setAnchorPoint(
        Vec2(0.5f, 0.5f)
    );

    _finalScoreLabel->setPosition(
        PulseCatch::CENTER_X,
        325.0f
    );

    _finalScoreLabel->setTextColor(Color4B::WHITE);
    _finalScoreLabel->setVisible(false);

    addChild(_finalScoreLabel, kChildTagHUDGameOver);

    _bestScoreLabel = Label::createWithTTF(
        "Best: 0",
        "fonts/arial.ttf",
        PulseCatch::RESULT_FONT_SIZE
    );

    _bestScoreLabel->setAnchorPoint(
        Vec2(0.5f, 0.5f)
    );

    _bestScoreLabel->setPosition(
        PulseCatch::CENTER_X,
        285.0f
    );

    _bestScoreLabel->setTextColor(Color4B::WHITE);
    _bestScoreLabel->setVisible(false);

    addChild(_bestScoreLabel, kChildTagHUDGameOver);
}

void GameScene::createInputListeners()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown =
        [this](Event* event)
        {
            auto* mouseEvent =
                static_cast<EventMouse*>(event);

            handlePointer(
                Vec2(
                    mouseEvent->getCursorX(),
                    mouseEvent->getCursorY()
                )
            );
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        mouseListener,
        this
    );
#else
    auto touchListener =
        EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan =
        [this](Touch* touch, Event*)
        {
            handlePointer(touch->getLocation());

            return true;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        touchListener,
        this
    );
#endif
}

void GameScene::startNewGame()
{
    _lives = PulseCatch::INITIAL_LIVES;
    _score = 0;

    _ringRadius = 0.0f;
    _targetLifetime = 0.0f;

    _playing = true;

    _ring->clear();
    clearTarget();

    _gameOverLabel->setVisible(false);
    _finalScoreLabel->setVisible(false);
    _bestScoreLabel->setVisible(false);

    updateHUD();

    spawnTarget();
}

void GameScene::spawnTarget()
{
    if (!_playing)
    {
        return;
    }

    const float minimumX =
        PulseCatch::TARGET_RADIUS;

    const float maximumX =
        PulseCatch::DESIGN_WIDTH -
        PulseCatch::TARGET_RADIUS;

    const float minimumY =
        PulseCatch::TARGET_RADIUS;

    const float maximumY =
        PulseCatch::DESIGN_HEIGHT -
        PulseCatch::TARGET_RADIUS;

    _targetPosition.set(
        RandomHelper::random_real(minimumX, maximumX),
        RandomHelper::random_real(minimumY, maximumY)
    );

    _targetLifetime =
        getCurrentVisibilityWindow();

    _target->clear();

    _target->drawSolidCircle(
        _targetPosition,
        PulseCatch::TARGET_RADIUS,
        0.0f,
        PulseCatch::TARGET_SEGMENTS,
        PulseCatch::CORAL_RED
    );
}

void GameScene::clearTarget()
{
    if (_target != nullptr)
    {
        _target->clear();
    }
}

void GameScene::update(float deltaTime)
{
    Scene::update(deltaTime);
    if (!_playing)
    {
        return;
    }

    updateRing(deltaTime);
    updateTargetLifetime(deltaTime);
}

void GameScene::updateRing(float deltaTime)
{
    _ringRadius +=
        getCurrentRingSpeed() * deltaTime;

    if (_ringRadius >
        PulseCatch::RING_MAX_RADIUS)
    {
        _ringRadius = 0.0f;
    }

    _ring->clear();

    if (_ringRadius > 0.0f)
    {
        _ring->drawCircle(
            PulseCatch::SCREEN_CENTER,
            _ringRadius,
            0.0f,
            PulseCatch::RING_SEGMENTS,
            false,
            PulseCatch::WHITE
        );
    }
}

void GameScene::updateTargetLifetime(float deltaTime)
{
    _targetLifetime -= deltaTime;

    if (_targetLifetime <= 0.0f)
    {
        clearTarget();

        loseLife();
    }
}

void GameScene::handlePointer(
    const Vec2& screenPosition
)
{
    if (!_playing)
    {
        startNewGame();

        return;
    }

    const float tapToTarget =
        screenPosition.distance(_targetPosition);

    // First check:
    // The tap must hit the active target dot.
    if (tapToTarget >
        PulseCatch::TARGET_RADIUS)
    {
        return;
    }

    const float targetDistanceFromCenter =
        PulseCatch::SCREEN_CENTER.distance(
            _targetPosition
        );

    const float pulseError =
        std::fabs(
            _ringRadius -
            targetDistanceFromCenter
        );

    clearTarget();

    // Second check:
    // The ring must be within the timing window.
    if (pulseError <=
        PulseCatch::PULSE_TOLERANCE)
    {
        ++_score;

        _sessionBestScore =
            std::max(
                _sessionBestScore,
                _score
            );

        updateHUD();

        spawnTarget();

        return;
    }

    // The target was hit, but the pulse timing
    // was outside the allowed tolerance.
    loseLife();
}

void GameScene::loseLife()
{
    --_lives;

    updateHUD();

    if (_lives <= 0)
    {
        enterGameOver();

        return;
    }

    spawnTarget();
}

void GameScene::enterGameOver()
{
    _playing = false;

    _ring->clear();
    clearTarget();

    _sessionBestScore =
        std::max(
            _sessionBestScore,
            _score
        );

    _gameOverLabel->setString(
        "GAME OVER"
    );

    _finalScoreLabel->setString(
        "Score: " +
        std::to_string(_score)
    );

    _bestScoreLabel->setString(
        "Best: " +
        std::to_string(_sessionBestScore)
    );

    _gameOverLabel->setVisible(true);
    _finalScoreLabel->setVisible(true);
    _bestScoreLabel->setVisible(true);
}

void GameScene::updateHUD()
{
    _livesLabel->setString(
        "Lives: " +
        std::to_string(_lives)
    );

    _scoreLabel->setString(
        "Score: " +
        std::to_string(_score)
    );
}

float GameScene::getCurrentRingSpeed() const
{
    const int speedSteps =
        _score /
        PulseCatch::SUCCESSFUL_TAPS_PER_SPEED_STEP;

    return
        PulseCatch::INITIAL_RING_SPEED +
        speedSteps *
        PulseCatch::SPEED_STEP;
}

float GameScene::getCurrentVisibilityWindow() const
{
    const int visibilitySteps =
        _score /
        PulseCatch::SUCCESSFUL_TAPS_PER_VISIBILITY_STEP;

    return std::max(
        PulseCatch::MIN_VISIBILITY_WINDOW,
        PulseCatch::INITIAL_VISIBILITY_WINDOW -
            visibilitySteps *
            PulseCatch::VISIBILITY_STEP
    );
}
