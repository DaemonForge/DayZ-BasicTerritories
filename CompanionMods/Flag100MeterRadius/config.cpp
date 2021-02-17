class CfgPatches
{
	class Flag100MeterRadius
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={
		};
	};
};
class CfgMods
{
	class Flag100MeterRadius
	{
		dir="Flag100MeterRadius";
        name="Flag100MeterRadius";
        credits="DaemonForge";
        author="";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={  "Game"};
	    class defs
	    {
			class gameScriptModule
            {
                value="";
                files[]={"Flag100MeterRadius/scripts/3_Game"};
            };
			
        };
    };
};
