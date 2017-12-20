// Bpn.h: interface for the CBpn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BPN_H__CB0F7ED8_8AD4_4FD5_B5D9_C60C939F5548__INCLUDED_)
#define AFX_BPN_H__CB0F7ED8_8AD4_4FD5_B5D9_C60C939F5548__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBpn  
{
private:
	int nLayerCnt;		// ���� ����: �Է� + ��� + ����
	int *NodeCnt;		// ���� ��� ��: NodeCnt[����ȣ]
	double **Node;		// ���: Node[����ȣ][��� ��ȣ]
	double ***Weight;	// ����ġ: Weight[����ȣ][����ȣ1][����ȣ2]
						// ����ȣ1�� ���� ���� ��� ��ȣ, ��� ��ȣ2�� ���� ���� ��� ��ȣ
	double **Delta;		// delta: Delta[����ȣ][����ȣ]
	double dGain;		// �̵�

public:
	CBpn(int nLayerCntInput, int *NodeCntInput, double dGainInput);	// ������: ����, ����, �̵� �Է� + �޸� �Ҵ�

	static double ActivationFn(double x);				// Ȱ�� �Լ�
	static double DifferentialActivationFn(double x);	// Ȱ�� �Լ��� �̺�

	void InitWeight();								// ����ġ �ʱ�ȭ
	int ComputeNet(double *Input);					// �Է¿� ���� ��� ���
	void ComputeDelta(double *Output);				// Delta ���
	void UpdateWeight();							// ����ġ ����
	int Train(double *Input, double *Output);		// �Ʒ�: ComputeNet + ComputeDelta + UpdateDelta
	virtual ~CBpn();

};

#endif // !defined(AFX_BPN_H__CB0F7ED8_8AD4_4FD5_B5D9_C60C939F5548__INCLUDED_)
