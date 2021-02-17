class CfgPatches
{
	class IncreaseRaidToolHealth
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={
			"DZ_Weapons_Melee_Blade"
		};
	};
};
class cfgVehicles
{
	class Inventory_Base;
	class FirefighterAxe: Inventory_Base
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
				};
			};
		};
	};
};