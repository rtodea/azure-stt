using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

int PushData(const string& filename, shared_ptr<PushAudioInputStream>& pushStream)
{
    try
    {
        WavFileReader reader(filename);

        vector<uint8_t> buffer(1000);
        // Read data and push them into the stream
        int readSamples = 0;
        while ((readSamples = reader.Read(buffer.data(), (uint32_t)buffer.size())) != 0)
        {
            // Push a buffer into the stream
            pushStream->Write(buffer.data(), readSamples);
        }

        // Close the push stream.
        pushStream->Close();
    }
    catch (const exception& e)
    {
        cout << "Error in pushing audio stream. " << e.what() << endl;
        return 1;
    }

    return 0;
}


char* recognize() {
    auto config =
            SpeechConfig::FromSubscription(
                    "YourSubscriptionKey",
                    "YourServiceRegion"
            );

    auto audioConfig = AudioConfig::FromStreamInput(pullStream);
    // Creates a push stream
    auto pushStream = AudioInputStream::CreatePushStream(
            AudioStreamFormat::GetCompressedFormat(AudioStreamContainerFormat::OGG_OPUS));

    // Creates an audio config object from stream input;
    auto audioInput = AudioConfig::FromStreamInput(pushStream);

    // Pushes audio into the voice profile client.
    auto error = PushData(trainingFilename, pushStream);
    auto recognizer = SpeechRecognizer::FromConfig(config, audioInput);
    auto result = recognizer->RecognizeOnceAsync().get();

    auto text = result->Text;
    return text;
}