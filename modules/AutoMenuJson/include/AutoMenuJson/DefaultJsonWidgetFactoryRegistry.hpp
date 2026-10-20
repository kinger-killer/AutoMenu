#pragma once

#include <memory>

#include <AutoMenuJson/JsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/BarJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/ButtonJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/CheckBoxJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/DropdownListJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/LabelButtonJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/LabelJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/RadioButtonJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/SliderJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/SpriteButtonJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/TextInputJsonWidgetFactory.hpp>
#include <AutoMenuJson/Factories/ToggleSwitchJsonWidgetFactory.hpp>

// The registry only connects JSON type names to the corresponding factory.
inline JsonWidgetFactoryRegistry createDefaultJsonWidgetFactoryRegistry()
{
	JsonWidgetFactoryRegistry factories;
	factories.add(std::make_unique<BarJsonWidgetFactory>());
	factories.add(std::make_unique<ButtonJsonWidgetFactory>());
	factories.add(std::make_unique<CheckBoxJsonWidgetFactory>());
	factories.add(std::make_unique<DropdownListJsonWidgetFactory>());
	factories.add(std::make_unique<LabelButtonJsonWidgetFactory>());
	factories.add(std::make_unique<LabelJsonWidgetFactory>());
	factories.add(std::make_unique<RadioButtonJsonWidgetFactory>());
	factories.add(std::make_unique<SliderJsonWidgetFactory>());
	factories.add(std::make_unique<SpriteButtonJsonWidgetFactory>());
	factories.add(std::make_unique<TextInputJsonWidgetFactory>());
	factories.add(std::make_unique<ToggleSwitchJsonWidgetFactory>());
	return factories;
}

inline const JsonWidgetFactoryRegistry& getDefaultJsonWidgetFactoryRegistry()
{
	static const JsonWidgetFactoryRegistry registry = createDefaultJsonWidgetFactoryRegistry();
	return registry;
}
