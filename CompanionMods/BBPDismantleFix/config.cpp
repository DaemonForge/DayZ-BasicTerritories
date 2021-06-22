class CfgPatches
{
	class BBPDismantleFix
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={
			"BaseBuildingPlus",
			"BasicTerritories"
		};
	};
};
class CfgMods
{
	class BBPDismantleFix
	{
		dir="BBPDismantleFix";
        name="BBPDismantleFix";
        credits="DaemonForge";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={ "World" };
	    class defs
	    {
			class worldScriptModule
            {
                value="";
                files[]={ "BBPDismantleFix/scripts/4_World" };
            };
			
        };
    };
};
