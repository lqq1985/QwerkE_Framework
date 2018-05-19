#ifndef _ViewerScene_H_
#define _ViewerScene_H_

#include "Scene.h"

// TODO: Turn into some model viewer scene maybe.
// Have a targeted camera that rotates around a stationary object.
class ViewerScene : public Scene
{
public:
	ViewerScene();
	~ViewerScene();

	void Initialize();
	void Draw();

private:
	void p_Update(double TimePassed);
};

#endif // !_ViewerScene_H_