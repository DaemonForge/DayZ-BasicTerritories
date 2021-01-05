


modded class ActionConstructor
{
	override void RegisterActions( TTypenameArray actions )
	{
		super.RegisterActions( actions );	
		actions.Insert( ActionAddTerritoryMember );
		actions.Insert( ActionResetTerritoryMembers );
		actions.Insert( ActionAcceptMembership );
	}
};