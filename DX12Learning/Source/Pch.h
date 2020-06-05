#pragma once

//========================.
//Windows�n.
//========================.

#include <Windows.h>
#include <wrl.h>

//========================.
//DirectX�n.
//========================.

#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//========================.
//STL
//========================.
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <filesystem>
#include <execution>
#include <any>

//========================.
//�f�o�b�O��.
//========================.
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG