#ifdef BASICMAP
class BasicTerritoryMapMarker extends BasicMapCircleMarker {
	bool IsOverLapping = false;
	
	void SetOverLaping(bool state = true){
		IsOverLapping = state;
	}
	
	void BasicTerritoryMapMarker(string name, vector pos, string icon = "", array<int> colour = NULL, int alpha = 235, bool onHUD = false) {
		Name = name;
		if (icon != ""){
			Icon = icon;
		}
		Pos = pos;
		if (colour != NULL){
			Colour = colour;
		}
		Alpha = alpha;
		Is3DMarker = onHUD;
		HideIntersects = false;
		ShowCenterMarker = true;
	}	
	
	override bool OnHUD(){
		return false;
	}
		
	override int GetColour(){
		if (IsOverLapping){
			return ARGB(190, 255, 44, 20);
		}
		return ARGB(190, 255, 140, 0);
	}
	
	
	
	override string GetIcon(){
		return "BasicMap\\gui\\images\\flag.paa";
	}
	
	override bool Editable(){
		return false;
	}
	
	override string GetGroup(){
		return BasicMap().CLIENT_KEY;
	}
}
#endif