#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int ready = 0;
int count = 0;
int pointer = 0;
Point tmpcoord;
Vec3b tempcolor;

void pressKey(WORD key) {
    cout << "Key" << endl;
    keybd_event(key, 0, 0, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void clickLeftMouseButton() {
    for (int i = 0; i < 4; ++i) {
        INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        Sleep(100);
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input, sizeof(INPUT));
        Sleep(200);
    }
}

void Restart() {
    Sleep(1000);
    pressKey(0x31); 
    Sleep(2000);
    clickLeftMouseButton();
    cout << "End Restart" << endl;
}

int main2() {
    Sleep(2000);
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

int main() {
    // Inicjalizacja
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    HDC hwindowDC = GetDC(NULL);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, screen_width, screen_height);
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), screen_width, -screen_height, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };

    // Współrzędne do rysowania i sprawdzania kolorów
    vector<Point> coordinates = {
        {1010, 533}, {1008, 546}, {1004, 560}, {997, 572}, {987, 582}, {975, 589},
        {962, 592}, {949, 591}, {936, 587}, {925, 578}, {913, 561}, {909, 547},
        {909, 533}, {913, 519}, {920, 507}, {930, 497}, {942, 490}, {956, 487},
        {963, 488}, {975, 490}, {987, 497}, {1001, 513}, {1006, 526}
    };

    int cx = screen_width / 2;
    int cy = screen_height / 2;

    // Współrzędne do sprawdzania kolorów
    vector<Point> coords2 = {
        {cx, cy + 5},
        {cx + 1, cy + 5},
        {cx + 2, cy + 5},
        {cx + 3, cy + 5},
        {cx - 1, cy + 5},
        {cx - 2, cy + 5},
    };

    Vec3b c0, c1, c2, c3, c4, c5;

    namedWindow("Zrzut ekranu", WINDOW_NORMAL);
    int count = 0;
    Point found_coord;
    bool found = false;
    Vec3b foundcolor;

    while (true) {
        // Przechwytywanie ekranu do macierzy OpenCV
        Mat screenshot = Mat::zeros(screen_height, screen_width, CV_8UC4);
        SelectObject(hwindowCompatibleDC, hbwindow);
        StretchBlt(hwindowCompatibleDC, 0, 0, screen_width, screen_height, hwindowDC, 0, 0, screen_width, screen_height, SRCCOPY);
        GetDIBits(hwindowCompatibleDC, hbwindow, 0, screen_height, screenshot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        // Konwersja koloru z BGRA na BGR
        cvtColor(screenshot, screenshot, COLOR_BGRA2BGR);

        // Pobieranie kolorów pikseli z coords2 i przypisanie ich do zmiennych c0, c1, c2, c3, c4, c5
        if (ready == 0) {
            c0 = screenshot.at<Vec3b>(coords2[0]);
            c1 = screenshot.at<Vec3b>(coords2[1]);
            c2 = screenshot.at<Vec3b>(coords2[2]);
            c3 = screenshot.at<Vec3b>(coords2[3]);
            c4 = screenshot.at<Vec3b>(coords2[4]);
            c5 = screenshot.at<Vec3b>(coords2[5]);

            // Konwersja wartości c0 na string
            string c0_text = "c0: [" + to_string((int)c0[2]) + ", " + to_string((int)c0[1]) + ", " + to_string((int)c0[0]) + "]";
            string c1_text = "c1: [" + to_string((int)c1[2]) + ", " + to_string((int)c1[1]) + ", " + to_string((int)c1[0]) + "]";
            string c2_text = "c2: [" + to_string((int)c2[2]) + ", " + to_string((int)c2[1]) + ", " + to_string((int)c2[0]) + "]";
            string c3_text = "c3: [" + to_string((int)c3[2]) + ", " + to_string((int)c3[1]) + ", " + to_string((int)c3[0]) + "]";
            string c4_text = "c4: [" + to_string((int)c4[2]) + ", " + to_string((int)c4[1]) + ", " + to_string((int)c4[0]) + "]";
            string c5_text = "c5: [" + to_string((int)c5[2]) + ", " + to_string((int)c5[1]) + ", " + to_string((int)c5[0]) + "]";

            // Wyświetlanie tekstu na obrazie
            putText(screenshot, c0_text, Point(cx + 200, cy - 325), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
            putText(screenshot, c1_text, Point(cx + 200, cy - 300), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
            putText(screenshot, c2_text, Point(cx + 200, cy - 275), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
            putText(screenshot, c3_text, Point(cx + 200, cy - 250), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
            putText(screenshot, c4_text, Point(cx + 200, cy - 225), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
            putText(screenshot, c5_text, Point(cx + 200, cy - 200), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);

            // Warunek sprawdzający kolory pikseli z coords2
            if (c0[1] > 175 && c0[0] < 90 && c0[2] < 90 && c1[1] > 150 && c2[1] > 150 && c3[1] > 150 && c4[1] > 150 && c5[1] > 150) {
                cout << "Mamy to!!!!" << endl;
                ready = 1;
            }
        }



        // Szukanie pierwszego piksela spełniającego warunki R > 200, G > 100, B < 50


        if (ready == 1) {

            if(pointer == 0)
            {
                for (const auto& coord : coordinates) {
                    Vec3b color = screenshot.at<Vec3b>(coord);
                    if (color[2] > 200 && color[1] > 80 && color[0] < 50) {  
                        found_coord = coord;
                        found = true;
                        if (pointer == 0) {
                            if (tmpcoord != coord) {
                                Sleep(5);
                                pointer = 1;
                                tmpcoord = coord;
                                tempcolor = screenshot.at<Vec3b>(Point(tmpcoord.x, tmpcoord.y));
                                cout << "Wykonanie programu" << endl;
                            }
                        }
                        break;
                    }
                }

                if (pointer == 0 && found == true && ready == 1) {
                    //Restart();
                    cout << "Restart?" << endl;
                }
            }

            Vec3b foundcolor = screenshot.at<Vec3b>(Point(tmpcoord.x, tmpcoord.y));
            putText(screenshot, "TMP Color: " + to_string(foundcolor[0]) + " , " + to_string(foundcolor[1]) + " , " + to_string(foundcolor[2]), Point(cx, cy - 400), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(foundcolor[0], foundcolor[1], foundcolor[2]), 1);

            circle(screenshot, { tmpcoord.x, tmpcoord.y }, 4, Scalar(0, 255, 0), -1);

            if (foundcolor != tempcolor && pointer > 0) {
                cout << "xOut" << endl;
                pressKey(0x45);
                pointer = 0;
                count++;
            }

            if (count == 3) {
                ready = 0;
                pointer = 0;
                found = false;
                tmpcoord = { 0,0 };
                count = 0;
                Restart();
            }



        }


        stringstream ss_ready;
        ss_ready << "Ready: " << ready;
        putText(screenshot, ss_ready.str(), Point(cx, cy - 350), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
        //cout << "Coord: " << tmpcoord << " Color: [" << (int)foundcolor[2] << ", " << (int)foundcolor[1] << ", " << (int)foundcolor[0] << "]" << pointer << endl;
        putText(screenshot, "Pointer: " + to_string(pointer), Point(cx, cy - 450), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
        putText(screenshot, "TMP Coord: " + to_string(tmpcoord.x) + " , " + to_string(tmpcoord.y), Point(cx, cy - 425), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
        putText(screenshot, "Found: " + to_string(found), Point(cx, cy - 375), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
        putText(screenshot, "Count: " + to_string(count), Point(cx, cy - 325), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);




        // Rysowanie punktów na obrazie
        for (const auto& coord : coordinates) {
            circle(screenshot, coord, 1, Scalar(0, 0, 255), -1);  // Czerwone kropki
        }

        for (const auto& coord : coords2) {
            circle(screenshot, coord, 1, Scalar(0, 0, 0), -1);  // Zielone kropki
        }

        // Wyświetlanie obrazu
        imshow("Zrzut ekranu", screenshot);

        // Oczekiwanie na naciśnięcie klawisza 'q' w celu wyjścia
        if (waitKey(1) == 'q') {
            break; 
        }

        if (waitKey(1) == 's') {
            ready = 0;
            pointer = 0;
            found = false;
            tmpcoord = { 0,0 };
            count = 0;
        }

    }

    // Zwalnianie zasobów
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(NULL, hwindowDC);

    return 0;
}
