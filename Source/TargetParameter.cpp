/*
  ==============================================================================

    TargetParameter.cpp
    Created: 2 Nov 2016 5:00:04pm
    Author:  bkupe

  ==============================================================================
*/

#include "TargetParameter.h"
#include "Engine.h"
#include "TargetParameterUI.h"

TargetParameter::TargetParameter(const String & niceName, const String & description, const String & initialValue, WeakReference<ControllableContainer> rootReference, bool enabled) :
	StringParameter(niceName,description,initialValue,enabled),
	useGhosting(true),
	rootContainer(rootReference),
	target(nullptr)
{
	if (rootContainer == nullptr) rootContainer = Engine::getInstanceWithoutCreating();
	type = TARGET;
}

TargetParameter::~TargetParameter()
{
	if (rootContainer != nullptr) rootContainer->removeControllableContainerListener(this);
	setValue("");
}

void TargetParameter::setGhostValue(const String & ghostVal)
{
	if (ghostVal == ghostValue) return;

	ghostValue = ghostVal;
	if (ghostValue.isNotEmpty())
	{
		if(rootContainer != nullptr) rootContainer->addControllableContainerListener(this);
	} else
	{
		if(!rootContainer.wasObjectDeleted()) rootContainer->removeControllableContainerListener(this);
	}

	
}

void TargetParameter::setValueFromTarget(Controllable * c)
{
	if (c == target) return;
	setValue(c->getControlAddress(rootContainer));
}

void TargetParameter::setValueInternal(var & newVal)
{
	StringParameter::setValueInternal(newVal);

	if (newVal.toString().isNotEmpty())
	{
		WeakReference<Controllable> c = rootContainer->getControllableForAddress(newVal.toString());
		if(c != nullptr) setTarget(c);
		else setGhostValue(newVal.toString());
	} else
	{
		setTarget(nullptr);
		setGhostValue(String::empty);
	}
}

void TargetParameter::setTarget(WeakReference<Controllable> c)
{
	if (target != nullptr)
	{
		if (!target.wasObjectDeleted()) target->removeControllableListener(this);
	}
	
	target = c;

	if (target != nullptr)
	{
		target->addControllableListener(this);
		setGhostValue(String::empty);
	}
}

void TargetParameter::childStructureChanged(ControllableContainer *)
{
	if (target == nullptr && ghostValue.isNotEmpty())
	{
		WeakReference<Controllable> c = rootContainer->getControllableForAddress(ghostValue);
		if (c != nullptr) setValueFromTarget(c);
	}
}

void TargetParameter::controllableRemoved(Controllable * c)
{
	if (c == target || target.wasObjectDeleted())
	{
		String oldValue = stringValue();
		setValue(String::empty);
		setGhostValue(oldValue);
	}
}

TargetParameterUI * TargetParameter::createTargetUI(TargetParameter * _target)
{
	return new TargetParameterUI(_target == nullptr ?this:_target);
}

ControllableUI * TargetParameter::createDefaultUI(Controllable * targetControllable)
{
	return createTargetUI((TargetParameter*)targetControllable);
}
