

export class MatchNotFoundException extends Error {

  constructor(message: string = "A partida não foi encotrada.") {
    super(message);
    this.name = "MatchNotFoundException";
  }

}

