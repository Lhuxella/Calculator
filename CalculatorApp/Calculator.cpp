#include "Calculator.h"
#include "imgui.h"
#include <iostream>
#include <array>
#include <string>
#include <algorithm>

namespace Calc
{
    
    char buf[32] = "";
    short bufCursor = 0;
    double rOperand = 0.0; 
    double result = 0.0;

    void RenderUI()
    {

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);

        ImGui::Begin("Primary UI", nullptr, ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

        ImGui::Text(std::to_string(result).c_str());
        static std::string operateSymbol = "";
        ImGui::SameLine(); ImGui::Text(operateSymbol.c_str());

        if (ImGui::InputText("Input", buf, IM_ARRAYSIZE(buf),
            ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank
            | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank))
        {
                result += std::atof(buf); clearBuffer();
        }

        ImGui::Text("");

        //TODO: Change to ENUM to make all symbols work or std::map
        const std::array<std::array<std::string, 4>, 6> buttonImgs{
            {
            { "%","CE","C", "<-" },
            { "1/x","x^2","SQRT", "/" },
            { "7","8","9", "x" },
            { "4","5","6", "-" },
            { "1","2","3", "+" },
            { "(+/-)","0",".", "=" }
            }
        };

        std::string currentButton = " ";

        //Calculator Buttons functionality.
        ImGui::BeginTable("ButtonGrid", 4);
        for (int row = 0; row < 6; row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 4; column++)
            {
                ImGui::TableSetColumnIndex(column);
                currentButton = buttonImgs[row][column];
 
                if (ImGui::Button(currentButton.c_str(), { 75,50 }))
                {
                    if (currentButton.size() == 1)
                    {
                        switch (currentButton.at(0))
                        {
                        case * "C": { clearBuffer(); result = 0; rOperand = 0; operateSymbol = ""; } break;
                        //case ("CE"[1]): { clearBuffer(); std::cout << "Hit"; } break;

                        case * "=": { determineOperation(*operateSymbol.c_str(), result); operateSymbol = ""; clearBuffer(); } break;
                        //TODO: if operateSymbol is set do equals = calculation then unset.

                        case * "+": {

                            if (operateSymbol != "")
                            {
                                determineOperation(*operateSymbol.c_str(), result);
                                operateSymbol = "+"; clearBuffer();
                            }
                            else
                            {
                                operateSymbol = "+"; result += std::atof(buf);
                                rOperand = std::atof(buf); clearBuffer();
                            }} break;

                        case * "-": {

                            if (operateSymbol != "")
                            {
                                determineOperation(*operateSymbol.c_str(), result);
                                operateSymbol = "-"; clearBuffer();
                            }
                            else
                            {
                                operateSymbol = "-"; result -= std::atof(buf);
                                rOperand = std::atof(buf); clearBuffer();
                            }} break;

                        case * "x": {

                            if (operateSymbol != "")
                            {
                                determineOperation(*operateSymbol.c_str(), result);
                                operateSymbol = "x"; clearBuffer();
                            }
                            else
                            {
                                operateSymbol = "x"; result = std::atof(buf);
                                rOperand = std::atof(buf); clearBuffer();
                            }
                        } break;
                        case * "/": {

                            if (atof(buf) != 0)
                            {
                                if (operateSymbol != "")
                                {
                                    determineOperation(*operateSymbol.c_str(), result);
                                    operateSymbol = "/"; clearBuffer();
                                }
                                else
                                {
                                    operateSymbol = "/"; result -= std::atof(buf);
                                    rOperand = std::atof(buf); clearBuffer();
                                }
                            }
                            
                        } break;

                        case * "%": {

                            if (operateSymbol != "")
                            {
                                operateSymbol = "%"; clearBuffer();
                                determineOperation(*operateSymbol.c_str(), result);
                            }
                            else
                            {
                                operateSymbol = "%"; result = std::atof(buf);
                                rOperand = std::atof(buf); clearBuffer();
                                
                            }



                        } break;

                        default: {
                            buf[bufCursor] = *currentButton.c_str(); bufCursor++;
                            std::cout << *currentButton.c_str();}
                        }
                    }
                    else if (currentButton.size() == 2)
                    {
                        switch (currentButton.at(0))
                        {
                        case * "CE": { clearBuffer(); } break;
                        case * "<": { if (bufCursor > 0) {bufCursor--; buf[bufCursor] = 0;}} break;
                        
                        default: { std::cerr << "No button error."; }
                        }
                    }
                    else if (currentButton.size() == 3)
                    {
                        switch (currentButton.at(0))
                        {
                        case * "1/x": { result = result / 10; clearBuffer(); } break;

                        case * "x^2": {result *= result;} break;
                        }
                    }
                    else if (currentButton.size() == 4)
                    {
                        switch (currentButton.at(0))
                        {
                        case * "SQRT": { result = sqrt(result); } break;
                        }
                    }
                    else if (currentButton.size() == 5)
                    {
                        if (buf[0] != '\0')
                        {
                            int tempBuffer = std::stoi(buf);
                            tempBuffer *= -1;
                            clearBuffer();

                            std::string s = std::to_string(tempBuffer);
                            if (tempBuffer != 0)
                            {
                                for (int i = 0; i < s.size(); i++)
                                {
                                    buf[i] = s[i];
                                }
                            }
                        }
                        bufCursor += strlen(buf);
                    }
                    else
                    {
                        std::cerr << "Button 'label' length error. " << std::endl;
                    }
                }
            } 
        }

        ImGui::EndTable();
        ImGui::End();

        //ImGui::ShowDemoWindow();
    }


    void clearBuffer()
    {
        for (int i = 0; i < sizeof(buf); i++) {buf[i] = 0;}
        bufCursor = 0;
    }


    void determineOperation(const char &operateSym, double &resultVal)
    {
        switch (operateSym)
        {
        case '+': { resultVal += std::atof(buf); } break;
        case '-': { resultVal -= std::atof(buf); } break;
        case 'x': { resultVal *= std::atof(buf); } break;
        case '/': { resultVal /= std::atof(buf); } break;
        case '%': { resultVal = fmod(resultVal, std::atof(buf)); } break;
        default: std::cerr << "Invalid operator error. " << std::endl;
        }
    }


}

