#pragma once
//trace program

//	cout << __func__ << __FILE__ << __LINE__ <<endl;

#define ___TRACE(...) fprintf(fout, "file[%s]line[%u]func[%s]::",__FILE__,__LINE__,__func__);\
fprintf(fout,__VA_ARGS__)