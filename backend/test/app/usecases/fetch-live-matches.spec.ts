import { FetchLiveMatches } from "../../../src/app/usecases/fetch-live-matches";
import { IMatchStatus } from "../../../src/domain/entities/match.entity";
import { MqttTopics } from "../../../src/domain/constants/mqtt-topics.constants";
import { FetchLiveMatchesUseCase } from "../../../src/app/usecases/fetch-live-matches.usecase";

const makeMatch = (overrides = {}) => ({
  id: 1,
  shortHomeTeamName: "BRA",
  shortAwayTeamName: "JPN",
  homeScore: 1,
  homeColor: "#EAE380",
  awayScore: 0,
  awayColor: "#FFFFFF",
  status: IMatchStatus.LIVE,
  minute: 41,
  date: "26/06/2026",
  time: "21:00",
  ...overrides,
});

const mockGateway = {
  getAllMatch: jest.fn(),
  getAllMatchToday: jest.fn(),
  getMatchFromId: jest.fn(),
};

const mockMqtt = {
  connect: jest.fn(),
  disconnect: jest.fn(),
  publish: jest.fn(),
  subscribe: jest.fn(),
  unsubscribe: jest.fn(),
};

describe("FetchLiveMatches", () => {
  let usecase: FetchLiveMatchesUseCase;

  beforeEach(() => {
    jest.clearAllMocks();
    usecase = new FetchLiveMatches(mockGateway as any, mockMqtt as any);
  });

  it("publica apenas partidas ao vivo", async () => {
    mockGateway.getAllMatchToday.mockResolvedValue([
      makeMatch({ id: 1, status: IMatchStatus.LIVE }),
      makeMatch({ id: 2, status: IMatchStatus.FINISHED }),
      makeMatch({ id: 3, status: IMatchStatus.SCHEDULED }),
    ]);

    const result = await usecase.execute();

    expect(result).toHaveLength(1);
    expect(result[0].id).toBe(1);
  });

  it("publica no tópico TODAY com a lista de partidas ao vivo", async () => {
    const liveMatch = makeMatch({ id: 1, status: IMatchStatus.LIVE });
    mockGateway.getAllMatchToday.mockResolvedValue([liveMatch]);

    await usecase.execute();

    expect(mockMqtt.publish).toHaveBeenCalledWith(
      MqttTopics.TODAY,
      JSON.stringify([liveMatch]),
    );
  });

  it("publica cada partida no seu tópico individual", async () => {
    const liveMatch = makeMatch({ id: 42, status: IMatchStatus.LIVE });
    mockGateway.getAllMatchToday.mockResolvedValue([liveMatch]);

    await usecase.execute();

    expect(mockMqtt.publish).toHaveBeenCalledWith(
      MqttTopics.DETAILS(42),
      JSON.stringify(liveMatch),
    );
  });

  it("retorna lista vazia se não houver partidas ao vivo", async () => {
    mockGateway.getAllMatchToday.mockResolvedValue([
      makeMatch({ status: IMatchStatus.FINISHED }),
    ]);

    const result = await usecase.execute();

    expect(result).toHaveLength(0);
    expect(mockMqtt.publish).toHaveBeenCalledWith(
      MqttTopics.TODAY,
      JSON.stringify([]),
    );
  });
});
