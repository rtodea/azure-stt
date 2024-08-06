# Using Azure STT via `spx` cli

## Installation Steps on Ubuntu

From [HERE](https://learn.microsoft.com/en-us/azure/ai-services/speech-service/spx-basics?tabs=linuxinstall%2Cterminal):

```bash
wget https://dot.net/v1/dotnet-install.sh -O dotnet-install.sh
chmod +x ./dotnet-install.sh
```

Install channel `6.0`

```bash
./dotnet-install.sh --chanel 6.0
```

## Setup your `.env`

```bash
cp .env.template .env
```

Load the `.env` in your current session

```bash
set -a; source .env; set +a
```

## Commands

```bash
spx config @key --set $AZURE_SPEECH_TO_TEXT_KEY
spx config @region --set $AZURE_SPEECH_TO_TEXT_REGION
```

```bash
spx recognize --file ../samples/one-voice-some-static.wav
```

## Sample Response

```txt
SPX - Azure Speech CLI, Version 1.38.0+958afb6ba65aa73f0afa9bb51060fb735220ed13
Copyright (c) 2022 Microsoft Corporation. All Rights Reserved.

  audio.input.file=wav/one-voice-some-static.wav
  audio.input.type=file
  diagnostics.config.log.file=log-{run.time}.log
  output.all.audio.input.id=true
  output.all.recognizer.recognized.result.text=true
  output.all.recognizer.session.started.sessionid=true
  service.config.key= a1da****************************
  service.config.region=eastus
  x.command=recognize
  x.input.path=@none

SESSION STARTED: 219bbb9bfcbe4418b866cd5f239d8be6

Connection CONNECTED...
RECOGNIZING: my dog's name is
RECOGNIZING: my dog's name is rover
RECOGNIZED: My dog's name is Rover.

SESSION STOPPED: 219bbb9bfcbe4418b866cd5f239d8be6
```