//
//  E002_C030_P330.h
//  E002_Template
//
//  Created by whitems
//
//

#ifndef SoundPhonics_Template_E002_C030_P330_h
#define SoundPhonics_Template_E002_C030_P330_h


#include "E002_Base_Reward.h"
#include "E002_C030_SoundManager.h"
#include "E002_Base_ExampleItem.h"

using namespace cocos2d;

class E002_C030_P330 : public E002_Base_Reward
{
private:
    
    std::vector<E002_Base_ExampleItem*>                      *mExamples;
    std::vector<std::string>                                 *mQuestions;
    std::vector<std::string>                                 *mPoints;
    std::vector<int>                                         *mCompleteTargetTags;
    std::vector<CCPoint>                                     *mTargets;
    
    E002_C030_SoundManager *mSndManager;
    
    E002_Base_ExampleItem *mTouchedItem;
    
    int mSuccessCount;
public:
    
    ~E002_C030_P330();
    E002_C030_P330();

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void startFlashAnimation();
    
    virtual bool onTouchBegan(CCPoint tp);
    virtual void onTouchMoved(CCPoint tp);
	virtual void onTouchEnded(CCPoint tp);
    
    virtual void onViewLoad();
    virtual void onViewLoaded();
    virtual void onFirstNarration();
    virtual void onFirstNarrationEnd();
    virtual void onNarrationFinishedCallback(std::string fileName);
    virtual void OnLastFrame(CJAnimation* pAnimation);
    
    // Navigation DELEGATE functions.
    virtual void onTouchedNavigationButtonAtIndex();
    virtual void onTouchedNavigationButtonAtInfo();
    virtual void onTouchedNavigationButtonAtPrev();
    virtual void onTouchedNavigationButtonAtNext();
    
    CREATE_FUNC(E002_C030_P330);
    
    void setQuestions();
    
    void createBlankPiece();
    void completeShowExamples();
    
    void intoTarget(CJSprite* sp, CJSprite *target);
    void checkAnswer(CCNode *node, int targetTag);
    
    void onSuccesses(CCNode *node);
    void onFails(CCNode *node);
    void onComplete();
    
    void playEffect(CCNode* node, int index);
    
    
    void removeObject();
    void setExampleZorder(CCNode *node, int depth);
    
    virtual void completeFirstGuide();
};

#endif
