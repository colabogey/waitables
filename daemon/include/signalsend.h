#pragma once
class signalsend
{
public:
	signalsend() = default;
	virtual ~signalsend() = default;
	void sendsignal(char* sigvalue);
};
