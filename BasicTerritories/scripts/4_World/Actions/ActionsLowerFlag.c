modded class ActionLowerFlag: ActionContinuousBase
{	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (super.ActionCondition(player, target, item)){
			TerritoryFlag theFlag = TerritoryFlag.Cast( target.GetObject() );
			PlayerBase thePlayer = PlayerBase.Cast(player);
			if ( theFlag && thePlayer && thePlayer.GetIdentity() ){
				return theFlag.CheckPlayerPermission(thePlayer.GetIdentity().GetId(), TerritoryPerm.LOWERFLAG);
			}
		}
		return false;
	}
	
};