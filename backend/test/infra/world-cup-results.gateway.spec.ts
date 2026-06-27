/// <reference types="jest" />
import axios from "axios";
import { WorldCupResultGateway } from "../../src/infra/gateways/world-cup-results.gateway";
import { IMatchStatus } from "../../src/domain/entities/match.entity";
import { MatchNotFoundException } from "../../src/infra/shared/exceptions";

jest.mock("axios");
const mockedAxios = axios as jest.Mocked<typeof axios>;

const makeFakeGame = (overrides = {}) => ({
  id: 1,
  startTime: "2026-06-26T21:00:00-03:00",
  statusGroup: 3,
  gameTime: 41,
  statusText: "Primeiro Tempo",
  gameTimeDisplay: "41'",
  homeCompetitor: {
    symbolicName: "BRA",
    score: 1,
    color: "#EAE380",
  },
  awayCompetitor: {
    symbolicName: "JPN",
    score: 0,
    color: "#233251",
    awayColor: "#FFFFFF",
  },
  ...overrides,
});

const makeFakeGameDetails = (overrides = {}) => ({
  ...makeFakeGame(),
  venue: { name: "Hard Rock Stadium (Miami)" },
  events: [
    {
      gameTimeDisplay: "23'",
      competitorId: 2379,
      eventType: { id: 1, name: "Gol" },
    },
    {
      gameTimeDisplay: "38'",
      competitorId: 2379,
      eventType: { id: 1, name: "Gol" },
    },
    {
      gameTimeDisplay: "41'",
      competitorId: 9999,
      eventType: { id: 2, name: "Cartão Amarelo" }, // não é gol, deve ser filtrado
    },
  ],
  ...overrides,
});
describe("WorldCupResultGateway", () => {
  let gateway: WorldCupResultGateway;
  let mockGet: jest.Mock;

  beforeEach(() => {
    mockGet = jest.fn();
    mockedAxios.create.mockReturnValue({ get: mockGet } as any);
    gateway = new WorldCupResultGateway();
  });

  afterEach(() => jest.clearAllMocks());

  describe("getAllMatch", () => {
    it("retorna lista de partidas mapeadas", async () => {
      mockGet.mockResolvedValue({ data: { games: [makeFakeGame()] } });

      const result = await gateway.getAllMatch();

      expect(result).toHaveLength(1);
      expect(result[0]).toMatchObject({
        id: 1,
        shortHomeTeamName: "BRA",
        shortAwayTeamName: "JPN",
        homeScore: 1,
        awayScore: 0,
        status: IMatchStatus.LIVE,
        minute: 41,
      });
    });

    it("mapeia status corretamente", async () => {
      const games = [
        makeFakeGame({ statusGroup: 3 }), // LIVE
        makeFakeGame({ id: 2, statusGroup: 4 }), // FINISHED
        makeFakeGame({ id: 3, statusGroup: 2 }), // SCHEDULED
      ];
      mockGet.mockResolvedValue({ data: { games } });

      const result = await gateway.getAllMatch();

      expect(result[0].status).toBe(IMatchStatus.LIVE);
      expect(result[1].status).toBe(IMatchStatus.FINISHED);
      expect(result[2].status).toBe(IMatchStatus.SCHEDULED);
    });

    it("usa awayColor alternativo quando igual ao homeColor", async () => {
      const game = makeFakeGame({
        homeCompetitor: { symbolicName: "BRA", score: 0, color: "#FFFFFF" },
        awayCompetitor: {
          symbolicName: "JPN",
          score: 0,
          color: "#233251",
          awayColor: "#FFFFFF",
        },
      });
      mockGet.mockResolvedValue({ data: { games: [game] } });

      const result = await gateway.getAllMatch();

      expect(result[0].awayColor).toBe("#233251");
    });

    it("formata data e hora no fuso de São Paulo", async () => {
      mockGet.mockResolvedValue({ data: { games: [makeFakeGame()] } });

      const result = await gateway.getAllMatch();

      expect(result[0].date).toBe("26/06/2026");
      expect(result[0].time).toBe("21:00");
    });

    it("usa score 0 quando score é null", async () => {
      const game = makeFakeGame({
        homeCompetitor: { symbolicName: "BRA", score: null, color: "#EAE380" },
        awayCompetitor: {
          symbolicName: "JPN",
          score: null,
          color: "#233251",
          awayColor: "#FFFFFF",
        },
      });
      mockGet.mockResolvedValue({ data: { games: [game] } });

      const result = await gateway.getAllMatch();

      expect(result[0].homeScore).toBe(0);
      expect(result[0].awayScore).toBe(0);
    });
  });

  describe("getAllMatchToday", () => {
    it("retorna apenas partidas de hoje", async () => {
      const today = new Date().toLocaleDateString("pt-BR", {
        timeZone: "America/Sao_Paulo",
        day: "2-digit",
        month: "2-digit",
        year: "numeric",
      });

      const todayISO = new Date().toISOString();
      const games = [
        makeFakeGame({ id: 1, startTime: todayISO }),
        makeFakeGame({ id: 2, startTime: "2026-01-01T00:00:00-03:00" }),
      ];
      mockGet.mockResolvedValue({ data: { games } });

      const result = await gateway.getAllMatchToday();
      expect(result).toHaveLength(1);
      expect(result[0].id).toBe(1);
    });
  });

  describe("getMatchFromId", () => {
    it("retorna detalhes da partida", async () => {
      mockGet.mockResolvedValue({ data: { game: makeFakeGameDetails() } });

      const result = await gateway.getMatchFromId(1);

      expect(result.venue).toBe("Hard Rock Stadium (Miami)");
      expect(result.gameTimeDisplay).toBe("41'");
      expect(result.statusText).toBe("Primeiro Tempo");
      expect(result.events).toHaveLength(2);
      expect(result.events[0].minute).toBe("23'");
    });

    it("lança MatchNotFoundException quando a API falha", async () => {
      mockGet.mockRejectedValue(new Error("Not found"));

      await expect(gateway.getMatchFromId(99999)).rejects.toThrow(
        MatchNotFoundException,
      );
    });
  });
});
