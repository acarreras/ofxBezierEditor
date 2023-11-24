//
//  draggableVertex.h
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//
#pragma once

class draggableVertex {
	public:
		glm::vec2 pos = glm::vec2(0, 0);
		bool bBeingDragged = false;
		bool bOver = false;
		bool bBeingSelected = false;

};
