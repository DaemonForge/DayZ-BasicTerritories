modded class ActionDismantlePart : ActionContinuousBase
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
	
	
	//For Backwards compabaility with dismantle fix
	protected int m_LastSync = 0;
	protected bool m_CanDismantleHere = true;
	protected vector m_LastCheckLocation = vector.Zero;
	protected int m_LastCheckLocationNextTime = 0;
	
	bool CanIDismantleHere(vector pos, string guid){
		Print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		Print("  ================ WARNING!! ================");
		Print("  ||   Using old version of Dismantle Fix  ||");
		Print("  ||   Check discord for new version.      ||");
		Print("  ||   Or notifiy the server owner/admin   ||");
		Print("  ===========================================");
		
		if (!TerritoryFlag.HasTerritoryPermAtPos(guid, TerritoryPerm.DISMANTLE, pos)){
			GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().DismantleWarningMessage, TerritoryIcons.DismantleWarning);
			return false;
		}
		return true;
	}
}