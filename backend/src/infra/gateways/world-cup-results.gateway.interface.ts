import { IMatch, IMatchDetails } from "../../domain/entities/match.entity";

export interface IWorldCupResultsGateway {
  getAllMatch(): Promise<IMatch[]>;
  getAllMatchToday(): Promise<IMatch[]>;
  getMatchFromId(id: number): Promise<IMatchDetails>;
}

export const IWorldCupResultsGateway = Symbol("IWorldCupResultsGateway");
