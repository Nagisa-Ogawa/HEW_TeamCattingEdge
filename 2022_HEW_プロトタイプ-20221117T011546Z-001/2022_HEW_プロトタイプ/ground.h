#pragma once

class ground
{
private:
	float vertex_lt;//頂点情報
	float vertex_rt;//頂点情報
	float vertex_lu;//頂点情報
	float vertex_ru;//頂点情報
public:
	ground() {}
	ground(float lt, float rt, float lu, float ru) { vertex_lt = lt; vertex_rt = rt; vertex_lu = lu; vertex_ru = ru;};

	void SetVertex(float lt, float rt, float lu, float ru) { vertex_lt = lt; vertex_rt = rt; vertex_lu = lu; vertex_ru = ru; }

	float GetVertex0(void) { return vertex_lt; }
	float GetVertex1(void) { return vertex_rt; }
	float GetVertex2(void) { return vertex_lu; }
	float GetVertex3(void) { return vertex_ru; }
};