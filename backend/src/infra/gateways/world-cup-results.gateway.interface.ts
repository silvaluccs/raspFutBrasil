import { IMatch, IMatchDetails } from "../../app/domain/entities/IMatch";

export interface IWorldCupResultsGateway {
  getAllMatch(): Promise<IMatch[]>
  getAllMatchToday(): Promise<IMatch[]>
  getMatchFromId(id: number): Promise<IMatchDetails>
}

export const IWorldCupResultsGateway = Symbol("IWorldCupResultsGateway");
