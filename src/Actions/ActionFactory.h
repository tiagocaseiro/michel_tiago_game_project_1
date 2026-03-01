#pragma once

#include "Actions/ActionManager.h"
#include "Actions/IAction.h"
#include "Model/Board.h"

#include "Tools/Logging.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

	using ActionFactoryFunc =
		std::function<std::unique_ptr<IAction>(Board&, const nlohmann::json&)>;

	class ActionFactoryRegistry
	{
	public:
		static ActionFactoryRegistry& Instance()
		{
			static ActionFactoryRegistry instance;
			return instance;
		}

		void Register(const std::string& type, ActionFactoryFunc func) { mFactories[type] = std::move(func); }

		std::unique_ptr<IAction> Create(const std::string& type,
			Board& board,
			const nlohmann::json& json) const
		{
			auto it = mFactories.find(type);
			if (it != mFactories.end())
			{
				return it->second(board, json);
			}
			Logging::FLogError("Unknown action type: {}", type);
			return nullptr;
		}

	private:
		std::unordered_map<std::string, ActionFactoryFunc> mFactories;
	};

	template <typename DerivedAction>
	class AutoRegisterAction : public IAction
	{
	public:
		AutoRegisterAction(Board& board, std::unique_ptr<IActionParameters> params) : IAction(board, std::move(params)) 
		{
			sRegistrar;
		}

	    std::string GetTypeName() override { return Logging::get_type_name<DerivedAction>(); }

	private:
		struct Registrar
		{
			Registrar()
			{
				static_assert(IsAction<DerivedAction>);
				ActionFactoryRegistry::Instance().Register(Logging::get_type_name<DerivedAction>(), &DerivedAction::FromJson);
			}
		};
		static Registrar sRegistrar;
	};

	template <typename DerivedAction>
	typename AutoRegisterAction<DerivedAction>::Registrar AutoRegisterAction<DerivedAction>::sRegistrar;
