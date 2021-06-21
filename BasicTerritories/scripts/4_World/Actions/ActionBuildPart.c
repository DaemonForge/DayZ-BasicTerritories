modded class ActionBuildPart: ActionContinuousBase
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
					if (!TerritoryFlag.HasTerritoryPermAtPos(theGUID, TerritoryPerm.BUILD, theTarget.GetPosition())){
						GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().BuildPartWarningMessage, TerritoryIcons.BuildPartWarning);
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		
		
		TerritoryFlag theFlag = TerritoryFlag.Cast( action_data.m_Target.GetObject() );
		if (!theFlag)
			return;
			
		
		Construction construction = theFlag.GetConstruction();
		
		string part_name = BuildPartActionData.Cast(action_data).m_PartType;
		
		if ( construction && construction.IsPartConstructed(part_name) && part_name == "base" ) {
			if (action_data.m_MainItem) {
				action_data.m_MainItem.DecreaseHealth( GetBasicTerritoriesConfig().BuildBonusSledgeDamage, false );
			}
		}
	}
}