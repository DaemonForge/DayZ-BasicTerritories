class CfgPatches
{
	class BasicTerritories
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={
#ifdef GAMELABS
			"GameLabs_Scripts",
#endif
#ifdef BASICMAP
			"BasicMap",
#endif 
			"BasicTerritoriesDefine",
			"JM_CF_Scripts"
		};
	};
};
class CfgMods
{
	class BasicTerritories
	{
		dir="BasicTerritories";
        name="BasicTerritories";
        credits="DaemonForge";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={  "Game", "World", "Mission"};
	    class defs
	    {
			class gameScriptModule
            {
                value="";
                files[]={"BasicTerritories/scripts/3_Game"};
            };
			
			class worldScriptModule
            {
                value="";
                files[]={ "BasicTerritories/scripts/4_World" };
            };
			
	        class missionScriptModule
            {
                value="";
                files[]={ "BasicTerritories/scripts/5_Mission" };
            };
        };
    };
};
