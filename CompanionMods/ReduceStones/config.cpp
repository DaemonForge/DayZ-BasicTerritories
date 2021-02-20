class CfgPatches
{
	class ReduceStones
	{
		requiredVersion=0.1;
		requiredAddons[]={
			"DZ_Gear_Camping"
		};
	};
};
class cfgVehicles
{
	class BaseBuildingBase;
	class TerritoryFlag: BaseBuildingBase
	{
		class Construction
		{
			class totem
			{
				class base
				{
					name = "$STR_CfgVehicles_Construction_Part_Base";
					is_base = 1;
					id = 1;
					required_parts[] = {};
					conflicted_parts[] = {};
					collision_data[] = {"base_min","base_max"};
					build_action_type = 16; //Change to 36 to make it buildable with shovel or 52 to make it buildable with shovel and sledge
					dismantle_action_type = 16;
					material_type = 1;
					class Materials
					{
						class Material1
						{
							type = "WoodenLog";
							slot_name = "Material_FPole_WoodenLog";
							quantity = 1;
							lockable = 0;
						};
					};
				};
				class support
				{
					name = "$STR_CfgVehicles_TerritoryFlag_Att_Category_Support";
					id = 2;
					required_parts[] = {"base"};
					conflicted_parts[] = {};
					collision_data[] = {"support_min","support_max"};
					build_action_type = 36;
					dismantle_action_type = 36;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "Stone";
							slot_name = "Material_FPole_Stones";
							quantity = 18;  //Change this to change stones
							lockable = 0;
						};
						class Material2
						{
							type = "WoodenLog";
							slot_name = "Material_FPole_WoodenLog2";
							quantity = 6; //Change this to change logs stage 2
							lockable = 0;
						};
					};
				};
				class pole
				{
					name = "$STR_CfgVehicles_TerritoryFlag_Att_Category_Pole";
					id = 3;
					required_parts[] = {"support"};
					conflicted_parts[] = {};
					collision_data[] = {"pole_min","pole_max"};
					build_action_type = 2;
					dismantle_action_type = 64;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "MetalWire";
							slot_name = "Material_FPole_MetalWire";
							quantity = 0;
							lockable = 1;
						};
						class Material2
						{
							type = "Rope";
							slot_name = "Material_FPole_Rope";
							quantity = 0;
							lockable = 1;
						};
						class Material3
						{
							type = "Nail";
							slot_name = "Material_FPole_Nails";
							quantity = 60;
							lockable = 0;
						};
						class Material4
						{
							type = "WoodenLog";
							slot_name = "Material_FPole_MagicStick";
							quantity = 3;
							lockable = 0;
						};
					};
				};
			};
		};
	};
};
class CfgSlots
{
	
	class Slot_Material_FPole_Stones
	{
		name = "Material_FPole_Stones";
		displayName = "Material_FPole_Stones";
		selection = "Material_FPole_Stones";
		ghostIcon = "stones";
		stackMax = 18; //you also need to change this
	};
	/*
    class Slot_Material_FPole_WoodenLog2
    {
        name = "Material_FPole_WoodenLog2";
        displayName = "Material_FPole_WoodenLog2";
        selection = "Material_FPole_WoodenLog2";
        ghostIcon = "woodenlog";
        stackMax = 6; //Change this for logs
    };
	
	
	
	
	
	
	*/
};