/*
  ==============================================================================

    ConditionManager.h
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONDITIONMANAGER_H_INCLUDED
#define CONDITIONMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "Condition.h"

class ConditionManager :
	public BaseManager<Condition>,
	public Condition::ConditionListener
{
public:
	juce_DeclareSingleton(ConditionManager, true)

	ConditionManager();
	~ConditionManager();

	void addItemInternal(Condition *, var data) override;
	void removeItemInternal(Condition *) override;

	BoolParameter * isValid;
	void checkAllConditions();

	bool areAllConditionsValid();
	int getNumEnabledConditions();
	int getNumValidConditions();

	void conditionValidationChanged(Condition *);

	class ConditionManagerListener
	{
	public:
		virtual ~ConditionManagerListener() {}
		virtual void conditionManagerValidationChanged(ConditionManager *) {}
	};


	ListenerList<ConditionManagerListener> conditionManagerListeners;
	void addConditionManagerListener(ConditionManagerListener* newListener) { conditionManagerListeners.add(newListener); }
	void removeConditionManagerListener(ConditionManagerListener* listener) { conditionManagerListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionManager)

};


#endif  // CONDITIONMANAGER_H_INCLUDED
