#include <windows.h>
#include "Header.h"


// globals 
wchar_t inputBuffer[10] = L"";
int inputIndex = 0;
int currentStep = 0;
int alignment = 0;

Document* Documents = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_CHAR: {

        if (appSettings.getState() == false) {
            
            if (wParam == VK_RETURN) {
                int value = _wtoi(inputBuffer);

                if (currentStep == 0) appSettings.setColumns(value);
                else if (currentStep == 1) appSettings.setLines(value);
                else if (currentStep == 2) {
                    appSettings.setChar(value);
                    appSettings.setState(true);
                    appSettings.validSettings();

                    Documents = new Document(1, appSettings.getColumns(), appSettings.getLines(), appSettings.getChars());

                }

                currentStep++;
                inputIndex = 0;
                inputBuffer[0] = L'\0';
            }
            else if (wParam == VK_BACK) {
                if (inputIndex > 0) {
                    inputIndex--;
                    inputBuffer[inputIndex] = L'\0';
                }
            }
            else if (wParam >= L'0' && wParam <= L'9') {
                if (inputIndex < 9) {
                    inputBuffer[inputIndex++] = (wchar_t)wParam;
                    inputBuffer[inputIndex] = L'\0';
                }
            }

            InvalidateRect(hwnd, NULL, TRUE);
        }
        else {

            Pages* p = Documents->getPage(Documents->getCurrentPage());
            Columns* c = p->getColumn(p->getCurrentColumns());
            lines* l = c->getLine(c->getCurrentLine());

            if (wParam == VK_RETURN) {

                bool result = c->incrementLine();
                if(result)
                l = c->getLine(c->getCurrentLine());
                else {
                 bool result2 = p->incrementColumns();

                 if (result2) {
                     c = p->getColumn(p->getCurrentColumns());
                     l = c->getLine(c->getCurrentLine());
                 }
                 else {
                     Document* copy2 = new Document(Documents);
                     copy2->setMaxPage(copy2->getMaxPage() * 2);
                     delete Documents;
                     Documents = new Document(copy2);
                     copy2->setMaxPage(copy2->getMaxPage() / 2);

                     delete copy2;

                     Documents->incrementPage();
                     p = Documents->getPage(Documents->getCurrentPage());
                     c = p->getColumn(p->getCurrentColumns());
                     l = c->getLine(c->getCurrentLine());

                 }

                }

            }
            else if (wParam == VK_BACK) {
                bool result = l->removeChar();

                if (result == false) {

                    bool result2 = c->decrementLine();
                    if (result2) {
                        l = c->getLine(c->getCurrentLine());
                        l->removeChar();
                    }
                    else {
                       bool result3 = p->decrementColumns();

                       if (result3) {
                           c = p->getColumn(p->getCurrentColumns());
                           l = c->getLine(c->getCurrentLine());
                       }
                       else {

                           Documents->decrementPage();
                           p = Documents->getPage(Documents->getCurrentPage());
                           c = p->getColumn(p->getCurrentColumns());
                           l = c->getLine(c->getCurrentLine());
                           l->removeChar();

                       }
                    }
                }

            }
            else if (wParam == VK_SPACE) {

                bool result = l->addChar((wchar_t)wParam);

                if (result == false) {

                    c->incrementLine();
                    l = c->getLine(c->getCurrentLine());

                    l->addChar((wchar_t)wParam);
                }

            }
            else {
                bool result = l->addChar((wchar_t)wParam); // for moving to next column when the current column is filled 

                // and also for addition characters 

                if (result == false) {
                    
                   wchar_t* sentence = wordIntegrity(l);      // function for taking care of the word interity 
                   bool copy;

                   if (sentence == nullptr) copy = false;
                   else copy = true;

                    bool result2 = c->incrementLine();

                    if (result2) {
                        l = c->getLine(c->getCurrentLine());

                        if (copy) {
                            int k = 0;

                            while (sentence[k] != '\0') {
                                l->addChar(sentence[k]);
                                k++;
                            }

                            
                        }

                        

                        l->addChar((wchar_t)wParam);
                    }
                    else {
                        bool result3 = p->incrementCol();
                       

                        if (result3) {

                            c = p->getColumn(p->getCurrentColumns());
                            l = c->getLine(c->getCurrentLine());

                            if (copy) {
                                int k = 0;

                                while (sentence[k] != '\0') {
                                    l->addChar(sentence[k]);
                                    k++;
                                }


                            }
                        }
                        else {

                            Document* copy2 = new Document(Documents);
                            copy2->setMaxPage(copy2->getMaxPage() * 2);
                            Documents = new Document(copy2);
                            copy2->setMaxPage(copy2->getMaxPage() / 2);
                            
                            delete copy2;

                            Documents->incrementPage();
                            p = Documents->getPage(Documents->getCurrentPage());
                            c = p->getColumn(p->getCurrentColumns());
                            l = c->getLine(c->getCurrentLine());

                            if (copy) {
                                int k = 0;

                                while (sentence[k] != '\0') {
                                    l->addChar(sentence[k]);
                                    k++;
                                }


                            }

                            l->addChar((wchar_t)wParam);

                        }

                        

                        l->addChar((wchar_t)wParam);
                    }

                    delete[] sentence;
                }

            }
            InvalidateRect(hwnd, NULL, TRUE);
        }


        return 0;
    }

    case WM_RBUTTONDOWN: {
        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);

        if (mouseY >= 50 && mouseY <= 80) {

            if (mouseX >= 20 && mouseX <= 50) {
                alignment = 1;
                // state left 
            }
            else if (mouseX >= 60 && mouseX <= 90) {
                alignment = 2;
                // state center 
            }
            else if (mouseX >= 100 && mouseX <= 130) {

                alignment = 3;
                // state right 
            }
            else if (mouseX >= 140 && mouseX <= 170) {
                alignment = 4;
                // state justify 
            }
            InvalidateRect(hwnd, NULL, TRUE);
        }

        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        RECT rect;
        GetClientRect(hwnd, &rect);



        HBRUSH bgBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, bgBrush);
        DeleteObject(bgBrush);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));


        if(appSettings.getState() == false ) {

            HFONT font = CreateFont(40, 0, 0, 0, 600, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Times New Roman");
            SelectObject(hdc, font);


            const wchar_t* text = L"THE ARACANUM EDITOR.";
            TextOutW(hdc, 450, 20, text, lstrlenW(text));

            DeleteObject(font);

            const wchar_t* question = L"";

            if (currentStep == 0) question = L"Input your number of Columns : ";   
            else if (currentStep == 1) question = L"Input your number of Lines : ";
            else if (currentStep == 2) question = L"Input your number of Characters: ";
            
            TextOutW(hdc, 430, 150, question, lstrlenW(question));
            SetTextColor(hdc, RGB(0, 0, 0));

            TextOutW(hdc, 970, 150, inputBuffer, lstrlenW(inputBuffer));

     

            const wchar_t* prompt = L"Settings Screen ";
            TextOutW(hdc, 534, 80, prompt, lstrlenW(prompt));



        }
        else {

            RECT titleRect = { 0, 0, rect.right, 40 };
            HBRUSH titleBrush = CreateSolidBrush(RGB(30, 30, 45));
            FillRect(hdc, &titleRect, titleBrush);
            DeleteObject(titleBrush);

            HFONT titleFont = CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");


            SelectObject(hdc, titleFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);

            const wchar_t* titleText = L"THE ARCANUM EDITOR";
            TextOutW(hdc, 560, 8, titleText, lstrlenW(titleText));

            DeleteObject(titleFont);

            // for creation of the title thing over the tops 

            RECT ribbonRect = { 0, 40, rect.right, 90 };
            HBRUSH ribbonBrush = CreateSolidBrush(RGB(240, 240, 240));
            FillRect(hdc, &ribbonRect, ribbonBrush);
            DeleteObject(ribbonBrush);

            HFONT btnFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE,
                FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
            SelectObject(hdc, btnFont);
            SetTextColor(hdc, RGB(0, 0, 0));

            HBRUSH btnBrush = CreateSolidBrush(RGB(210, 210, 210));
            HBRUSH brdrBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);

            // buttons for alignment
            RECT btnL = { 20, 50, 50, 80 };
            FillRect(hdc, &btnL, btnBrush);
            FrameRect(hdc, &btnL, brdrBrush);
            TextOutW(hdc, 30, 55, L"L", 1);


            RECT btnC = { 60, 50, 90, 80 };
            FillRect(hdc, &btnC, btnBrush);
            FrameRect(hdc, &btnC, brdrBrush);
            TextOutW(hdc, 70, 55, L"C", 1);

            RECT btnR = { 100, 50, 130, 80 };
            FillRect(hdc, &btnR, btnBrush);
            FrameRect(hdc, &btnR, brdrBrush);
            TextOutW(hdc, 110, 55, L"R", 1);

            RECT btnJ = { 140, 50, 170, 80 };
            FillRect(hdc, &btnJ, btnBrush);
            FrameRect(hdc, &btnJ, brdrBrush);
            TextOutW(hdc, 150, 55, L"J", 1);

            DeleteObject(btnBrush);
            DeleteObject(btnFont);

            SetTextColor(hdc, RGB(0, 0, 0));
            HFONT textF = CreateFont(18, 0, 0, 0, 500, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Times New Roman");
            SelectObject(hdc, textF);


            // applying alignment changes only to this part over here 

            
            


            int totalCols = appSettings.getColumns();
            int totalLines = appSettings.getLines();
            Pages* currentPage = Documents->getPage(Documents->getCurrentPage());


            for (int colIdx = 0; colIdx < totalCols; colIdx++) {

                Columns* currentCol = currentPage->getColumn(colIdx);

                int xPos = 20 + (colIdx * ((appSettings.getChars() * 8) + 30));

                for (int lineIdx = 0; lineIdx < totalLines; lineIdx++) {
                    lines* currentLine = currentCol->getLine(lineIdx);

                    if (alignment == 2) {
                        int columnWidth = appSettings.getChars() * 8;
                        SIZE textSize;
                        
                        GetTextExtentPoint32W(hdc, currentLine->getSentence(), currentLine->getCurrent(), &textSize);
                        int centeredX = 20 + ((colIdx * columnWidth) + (columnWidth - textSize.cx) / 2);
                        int yPos = 110 + (lineIdx * 20);
                        TextOutW(hdc, centeredX, yPos, currentLine->getSentence(), currentLine->getCurrent());
                    }
                    else if (alignment == 3) {
                        int columnWidth = appSettings.getChars() * 8;
                        SIZE textSize;

                        GetTextExtentPoint32W(hdc, currentLine->getSentence(), currentLine->getCurrent(), &textSize);
                        int centeredX = 20 + ( (colIdx * columnWidth) + (columnWidth - textSize.cx) );
                        int yPos = 110 + (lineIdx * 20);
                        TextOutW(hdc, centeredX, yPos, currentLine->getSentence(), currentLine->getCurrent());
                    }
                    else if (alignment == 4 && currentLine->integrityMaintained() == true ) {

                        // justicification logic
                        /*
                        * If the number of characters is larger than 0 the calculate the text size and number of space 
                        * then calculate the left over space and divide it among all the spaces 
                        * create an array for storing the size of characters not the whole sentence 
                        * create a loop and whenever a space is encountered in the array store the left over space 
                        * which you divided on the spaces and also the remaining space 
                        * important update to make it like the ms word the justifying effect is only going to be upplied 
                        * when the word intgrty phenmna happnd 
                        */


                        int columnWidth = appSettings.getChars() * 8;
                        int yPos = 110 + (lineIdx * 20);
                        
                        if (currentLine->getCurrent() > 0) {
                            SIZE textSize;
                            GetTextExtentPoint32W(hdc, currentLine->getSentence(), currentLine->getCurrent(), &textSize);

                            int spaceCount = 0;

                            for (int i = 0; i < currentLine->getCurrent(); i++) {
                                if (currentLine->getSentence()[i] == L' ') spaceCount++;
                            }

                            int* dxArray = (int*)GlobalAlloc(GPTR, currentLine->getCurrent() * sizeof(int));

                            if (dxArray) {
                                for (int i = 0; i < currentLine->getCurrent(); i++) {
                                    SIZE charSize;
                                    GetTextExtentPoint32W(hdc, &currentLine->getSentence()[i], 1, &charSize); // try get current 
                                    dxArray[i] = charSize.cx;
                                }

                                int total = columnWidth - textSize.cx;


                                if (spaceCount > 0 && total > 0) {
                                    
                                    int extraSpace = total / spaceCount;
                                    int remainingPixels = total % spaceCount;

                                    for (int i = 0; i < currentLine->getCurrent(); i++) {
                                        
                                        if (currentLine->getSentence()[i] == L' ') {

                                            dxArray[i] += extraSpace;

                                            if (remainingPixels > 0) {
                                                dxArray[i]++;
                                                remainingPixels--;
                                            }

                                        }

                                    }

                                }

                                RECT columnRect = { xPos, yPos, xPos + columnWidth , yPos + 20 }; 
                                


                                ExtTextOutW(hdc, xPos, yPos, ETO_CLIPPED, &columnRect, currentLine->getSentence(), currentLine->getCurrent(), dxArray);
                                
                                GlobalFree(dxArray);
                            }

                        }

                    }
                    else {

                        int yPos = 110 + (lineIdx * 20);
                        int columnWidth = appSettings.getChars() * 8;
                        int* dxArray = (int*)GlobalAlloc(GPTR, currentLine->getCurrent() * sizeof(int));

                        for (int i = 0; i < currentLine->getCurrent(); i++) {
                            SIZE charSize;
                            GetTextExtentPoint32W(hdc, &currentLine->getSentence()[i], 1, &charSize); // try get current 
                            dxArray[i] = charSize.cx;
                        }

                        RECT columnRect = { xPos, yPos, xPos + columnWidth , yPos + 20 };

                        ExtTextOutW(hdc, xPos, yPos, ETO_CLIPPED, &columnRect, currentLine->getSentence(), currentLine->getCurrent(), dxArray);

                        GlobalFree(dxArray);

                       


                    }

                    // calculating the yposition 

                }

            }
            
            DeleteObject(textF);

        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"FastianWindow";


    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);


    RegisterClassW(&wc);


    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Blackwood & Hargrove",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1440, 720,
        NULL, NULL, hInstance, NULL
    );


    if (!hwnd) return 0;


    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}