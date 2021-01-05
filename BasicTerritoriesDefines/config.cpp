class CfgPatches
{
	class BasicTerritoriesDefine
	{
		requiredVersion = 0.1;
		requiredAddons[] = {};
	};
};

class CfgMods
{
	class BasicTerritoriesDefine
	{
		dir="BasicTerritoriesDefine";
        name="BasicTerritories";
        type="mod";
	    dependencies[]={"GameLib", "Game", "World", "Mission"};
	    class defs
	    {
			class gameLibScriptModule
			{
				value = "";
				files[] = {
					"BasicTerritoriesDefine/scripts/Common"
					};
			};
			class gameScriptModule
            {
				value = "";
                files[]={
					"BasicTerritoriesDefine/scripts/Common"
					};
            };
			
			class worldScriptModule
            {
                value="";
                files[]={ 
					"BasicTerritoriesDefine/scripts/Common"
				};
            };
			
	        class missionScriptModule
            {
                value="";
                files[]={
					"BasicTerritoriesDefine/scripts/Common"
				};
            };
        };
    };
};