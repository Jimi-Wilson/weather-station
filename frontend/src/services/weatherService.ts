import {apiClient} from "@/services/api.ts";
import type {PaginatedWeatherResponse, WeatherReading, WeatherStats} from "@/services/types.ts";
import type { AxiosResponse } from 'axios';

export const weatherService = {
    getLatestWeather: async (): Promise<WeatherReading> => {
        const response = await apiClient.get('/weather/latest');
        return response.data;
    },

    getRecentWeather: async (): Promise<WeatherReading[]> => {
        const allResults: WeatherReading[] = [];
        try {
            let nextUrl: string | null = 'weather/recent/?hours=24';

            while (nextUrl) {
                const response: AxiosResponse<PaginatedWeatherResponse> = await apiClient.get(nextUrl);

                if (response.data?.results) {
                    allResults.push(...response.data.results);
                }

                nextUrl = response.data.next;
            }
        } catch (error) {
            console.error('Error fetching recent weather data:', error);
        }

        return allResults;
    },

    getHistoricalWeather: async (startDateTime: string, endDateTime: string): Promise<WeatherReading[]> => {
        const response = await apiClient.get<PaginatedWeatherResponse>('/weather/historical', {
            params: {
                start_datetime: startDateTime,
                end_datetime: endDateTime
            }
        });
        return response.data.results;
    },

    getWeatherDataStats: async (hours: number = 24): Promise<WeatherStats> => {
        const response = await apiClient.get('/weather/stats', {
            params: {
                hours: hours
            }
        });
        return response.data;
    }
}


