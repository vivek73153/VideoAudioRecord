//
//  THSlideSwitchButton.cpp
//  PackageApp
//
//  Created by pureye4u BX on 12. 7. 10..
//  Copyright CJ Educations 2012. All rights reserved.
//

#include "THSlideSwitchButton.h"
#include "THMaskTargetMoveTo.h"

using namespace cocos2d;

THSlideSwitchButton::THSlideSwitchButton()
{
	m_hitArea = CCRectZero;
}

THSlideSwitchButton::~THSlideSwitchButton()
{
	this->removeAllChildrenWithCleanup(true);
}

bool THSlideSwitchButton::init(const char *pszMaskImage, const char *pszSwitchImage, const char *pszEffectImage, const char *pszThumbImage, int iTouchPriority)
{
	CCSprite *maskImage = CCSprite::create(pszMaskImage);
	m_hitArea.size = maskImage->getContentSize();
	m_hitArea.origin.x = - m_hitArea.size.width / 2;
	m_hitArea.origin.y = - m_hitArea.size.height / 2;
	
	if(!CCLayerColor::create(ccc4(0, 0, 0, 0), m_hitArea.size.width, m_hitArea.size.height))
	{
		return false;
	}
	
	// Initialize
	m_isOn = false;
	m_isScrolling = false;
	m_returnToMin = false;
	m_returnToMax = false;
	m_direction = 0;
	m_limitMinX = 0;
	m_limitMaxX = 0;
	m_scrollDistance = CCPointZero;
	CCPoint p = CCPointZero;
	
	CCSprite *switchImage = CCSprite::create(pszSwitchImage);
	
	m_masked = new THMask();
	m_masked->init();
	m_masked->setTarget(switchImage);
	m_masked->setMask(maskImage);
	this->addChild(m_masked);
	
	if(pszEffectImage)
	{
		CCSprite *effectImage = CCSprite::create(pszEffectImage);
		this->addChild(effectImage);
	}
	
	if(pszThumbImage)
	{
		m_thumb = new CCSprite();
		m_thumb->initWithFile(pszThumbImage);
		this->addChild(m_thumb);
	}
	
	
	m_limitMinX = MIN(0, m_hitArea.size.width - switchImage->getContentSize().width);
	
	
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, iTouchPriority, true);
	
	
	return true;
}

bool THSlideSwitchButton::getValue()
{
	return m_isOn;
}

void THSlideSwitchButton::setValue(bool isOn, bool withAnimation)
{
	if(m_isOn != isOn)
	{
		m_isOn = isOn;
		m_delegate->switchValueChanged(this);
	}
	
	if(m_isOn)
	{
		this->setToOn();
	}
	else
	{
		this->setToOff();
	}
}

void THSlideSwitchButton::setDelegate(THSwitchButtonDelegate *delegate)
{
	m_delegate = delegate;
}

void THSlideSwitchButton::setToOn()
{
	CCPoint p = CCPointZero;
	p.x = m_limitMaxX;
	m_masked->runAction(CCEaseSineInOut::create(THMaskTargetMoveTo::create(0.2, p)));
	m_thumb->runAction(CCEaseSineInOut::create(CCMoveTo::create(0.2, p)));
}

void THSlideSwitchButton::setToOff()
{
	CCPoint p = CCPointZero;
	p.x = m_limitMinX;
	m_masked->runAction(CCEaseSineInOut::create(THMaskTargetMoveTo::create(0.2, p)));
	m_thumb->runAction(CCEaseSineInOut::create(CCMoveTo::create(0.2, p)));
}

void THSlideSwitchButton::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool THSlideSwitchButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint p = this->convertTouchToNodeSpace(pTouch);
	
	if(m_hitArea.containsPoint(p))
	{
		m_isScrolling = true;
		m_returnToMin = false;
		m_returnToMax = false;
		m_direction = 0;
		m_scrollDistance.x = m_thumb->getPosition().x - p.x;
		
		return true;
	}
	
	return false;
}

void THSlideSwitchButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint p = this->convertTouchToNodeSpace(pTouch);
	
	if(m_isScrolling)
	{
		CCPoint currentPosition = m_thumb->getPosition();
		CCPoint top = CCPointMake(p.x + m_scrollDistance.x, currentPosition.y);
		m_direction = top.x - currentPosition.x;
		
		if(top.x < m_limitMinX)
		{
			top.x = m_limitMinX;
		}
		else if(top.x > m_limitMaxX)
		{
			top.x = m_limitMaxX;
		}
		else
		{
			m_returnToMin = false;
			m_returnToMax = false;
		}
		
		m_masked->setTargetPosition(top);
		m_thumb->setPosition(top);
	}
}

void THSlideSwitchButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(m_returnToMin || m_direction < 0)
	{
		this->setValue(false, true);
		m_returnToMin = false;
		m_direction = 0;
	}
	else if(m_returnToMax || m_direction > 0)
	{
		this->setValue(true, true);
		m_returnToMax = false;
		m_direction = 0;
	}
	else
	{
		if(m_isOn)
		{
			this->setValue(false, true);
		}
		else
		{
			this->setValue(true, true);
		}
	}
}

void THSlideSwitchButton::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	this->ccTouchEnded(pTouch, pEvent);
}

