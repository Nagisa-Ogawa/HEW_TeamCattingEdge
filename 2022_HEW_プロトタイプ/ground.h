#pragma once

class ground
{
private:
	float vertex_lt;//���_���
	float vertex_rt;//���_���
	float vertex_lu;//���_���
	float vertex_ru;//���_���
public:
	ground() {}
	ground(float lt, float rt, float lu, float ru) { vertex_lt = lt; vertex_rt = rt; vertex_lu = lu; vertex_ru = ru;};

	void SetVertex(float lt, float rt, float lu, float ru) { vertex_lt = lt; vertex_rt = rt; vertex_lu = lu; vertex_ru = ru; }

	float GetVertex0(void) { return vertex_lt; }
	float GetVertex1(void) { return vertex_rt; }
	float GetVertex2(void) { return vertex_lu; }
	float GetVertex3(void) { return vertex_ru; }
};