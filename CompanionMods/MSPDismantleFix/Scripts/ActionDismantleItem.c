modded class ActionDismantleItem: ActionContinuousBase
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		ItemBase theTarget;
		if (super.ActionCondition( player, target, item )){
			if (GetGame().IsServer()){
				return true;
			}
			if ( Class.CastTo( theTarget, target.GetObject() ) || Class.CastTo( theTarget, target.GetParent()) ) {
				PlayerBase thePlayer = PlayerBase.Cast(player);
				if (theTarget && thePlayer) {
					string theGUID = "";
					if ( thePlayer.GetIdentity() ) {
						theGUID = thePlayer.GetIdentity().GetId();
					}
					if (!TerritoryFlag.HasTerritoryPermAtPos(theGUID, TerritoryPerm.DISMANTLE, theTarget.GetPosition())){
						GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().DismantleWarningMessage, TerritoryIcons.DismantleWarning);
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}
	
}