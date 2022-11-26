int my_func(int param1, int arg1, int arg2)
{
	int result = 0;
	enum action
{
	SUM = 1,
	SUB,
	MUL,
	DIV,
};
	switch (param1)
	{
		case SUM:
		{
			result = arg1 + arg2;
			break;
		}
		case SUB:
		{
			result = arg1 - arg2;
			break;
		}
		case MUL:
		{
			result = arg1 * arg2;
			break;
		}
		case DIV:
		{
			result = arg1 / arg2;
			break;
		}
		default:
		{
			return 0;
		}
	}
	return result;
}