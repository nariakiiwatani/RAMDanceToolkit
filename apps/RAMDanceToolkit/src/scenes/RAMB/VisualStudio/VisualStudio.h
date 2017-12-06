#include "ramMain.h"
#include "ramMotionExtractor.h"
#include "BaseSceneWithJsonSettings.h"

class VisualStudio : public BaseSceneWithJsonSettings{
public:
    
    VisualStudio();
    virtual ~VisualStudio(){};
    
    void setupControlPanel();
    void drawImGui();
    
    void update();
    void draw();
    bool checkSetPort;
    
    inline string getName() const {
        return "VisualStudio";
    }
    
	void onEnable();
	void loadJson(const ofJson &json);
	ofJson toJson() const;

private:
    
	rdtk::MotionExtractor    motionExtractor;
    
    float mBox1H;
    float mBox2H;
    float mBox3H;
    float mBox4H;
    
    void example_drawBars(int index1, int index2, ofColor color, float numX, float numY, float height);
    float example_drawUnderbars(int index1, int index2, int index3, int index4, ofColor color1, ofColor color2, ofColor color3, ofColor color4, float numX1, float numY1, float numX2, float numY2, float height);
    
};


