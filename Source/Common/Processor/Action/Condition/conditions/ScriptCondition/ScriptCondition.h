/*
  ==============================================================================

    ScriptCondition.h
    Created: 21 Feb 2017 11:28:06am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../Condition.h"


class ScriptCondition :
	public Condition
{
public:
	ScriptCondition(var params);
	~ScriptCondition();
	
	static String conditionTemplate;
	Script script;

	//Script
	static var setValidFromScript(const var::NativeFunctionArgs &a);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	String getTypeString() const override { return ScriptCondition::getTypeStringStatic(); }
	static String getTypeStringStatic() { return "Script"; }
};

