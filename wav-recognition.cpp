//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

// <toplevel>
#include <speechapi_cxx.h>
#include <fstream>
#include "wav_file_reader.h"
#include <vector>
#include <future>

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;
// </toplevel>

void SpeechContinuousRecognitionWithFile()
{
    // <SpeechContinuousRecognitionWithFile>
    // Creates an instance of a speech config with specified subscription key and service region.
    // Replace with your own subscription key and service region (e.g., "westus").
    auto azureSpeechKey = std::getenv("AZURE_SPEECH_TO_TEXT_KEY");
    auto azureSpeechRegion = std::getenv("AZURE_SPEECH_TO_TEXT_REGION");
    // auto config = SpeechConfig::FromSubscription(azureSpeechKey, azureSpeechRegion);

    auto config = SpeechConfig::FromSubscription("a1da6dd8924a4366bdccf9346cd5322d", "eastus");
    config->SetProperty("OPENSSL_DISABLE_CRL_CHECK", "true");

    // Creates a speech recognizer using file as audio input.
    // Replace with your own audio file name.
    auto audioInput = AudioConfig::FromWavFileInput("./samples/whatstheweatherlike.wav");
    auto recognizer = SpeechRecognizer::FromConfig(config, audioInput);

    // promise for synchronization of recognition end.
    promise<void> recognitionEnd;

    // Subscribes to events.
    recognizer->Recognizing.Connect([] (const SpeechRecognitionEventArgs& e)
                                    {
                                        cout << "Recognizing:" << e.Result->Text << std::endl;
                                    });

    recognizer->Recognized.Connect([] (const SpeechRecognitionEventArgs& e)
                                   {
                                       if (e.Result->Reason == ResultReason::RecognizedSpeech)
                                       {
                                           cout << "RECOGNIZED: Text=" << e.Result->Text << "\n"
                                                << "  Offset=" << e.Result->Offset() << "\n"
                                                << "  Duration=" << e.Result->Duration() << std::endl;
                                       }
                                       else if (e.Result->Reason == ResultReason::NoMatch)
                                       {
                                           cout << "NOMATCH: Speech could not be recognized." << std::endl;
                                       }
                                   });

    recognizer->Canceled.Connect([&recognitionEnd](const SpeechRecognitionCanceledEventArgs& e)
                                 {
                                     cout << "CANCELED: Reason=" << (int)e.Reason << std::endl;

                                     if (e.Reason == CancellationReason::Error)
                                     {
                                         cout << "CANCELED: ErrorCode=" << (int)e.ErrorCode << "\n"
                                              << "CANCELED: ErrorDetails=" << e.ErrorDetails << "\n"
                                              << "CANCELED: Did you update the subscription info?" << std::endl;

                                         recognitionEnd.set_value(); // Notify to stop recognition.
                                     }
                                 });

    recognizer->SessionStopped.Connect([&recognitionEnd](const SessionEventArgs& e)
                                       {
                                           cout << "Session stopped.";
                                           recognitionEnd.set_value(); // Notify to stop recognition.
                                       });

    // Starts continuous recognition. Uses StopContinuousRecognitionAsync() to stop recognition.
    recognizer->StartContinuousRecognitionAsync().get();

    // Waits for recognition end.
    recognitionEnd.get_future().get();

    // Stops recognition.
    recognizer->StopContinuousRecognitionAsync().get();
    // </SpeechContinuousRecognitionWithFile>
}

int main(int argc, char **argv) {
    std::cout << "Usage: ./samples/whatstheweatherlike.wav" << std::endl;
    SpeechContinuousRecognitionWithFile();

    return 0;
}
