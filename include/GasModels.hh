#ifndef GasModels_hh
#define GasModels_hh


class DegradModel;
class HeedInterfaceModel;
class HeedOnlyModel;
class GasModelsMessenger;
class DetectorContruction;

class GasModels{
	public:
	
	GasModels(DetectorConstruction*);
	~GasModels();

	DegradModel* GetDegradModel(){return dm;};
	HeedInterfaceModel* GetHeedInterfaceModel(){return him;};
	HeedOnlyModel* GetHeedOnlyModel(){return hom;};
	
	private:
	GasModelsMessenger* fMessenger;
	DegradModel *dm;
	HeedInterfaceModel* him;
	HeedOnlyModel* hom;
    
    
	
};

#endif