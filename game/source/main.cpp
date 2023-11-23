// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "StrongholdRoyale.h"

#include <Windows.h>

#include <stdexcept>

int main()
{
	try
	{
		StrongholdRoyale game;
		game.start();
	}
	catch (const std::runtime_error& error)
	{
		MessageBoxA(nullptr, "Stronghold Royale: FATAL", error.what(), MB_OK);
	}
	catch (const std::exception& error)
	{
		MessageBoxA(nullptr, "Stronghold Royale: std::FATAL", "Unknown std error.", MB_OK);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "Stronghold Royale: unknown FATAL", "Unknown error. For details look into the log.", MB_OK);
	}

	return 0;
}