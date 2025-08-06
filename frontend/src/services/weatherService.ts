import {apiClient} from "@/services/api.ts";
import type {WeatherReading, WeatherStats} from "@/services/types.ts";

export const weatherService = {
    getLatestWeather: async (): Promise<WeatherReading> => {
        const response = await apiClient.get('/weather/latest');
        return response.data;
    },

    getRecentWeather: async (hours: number = 24): Promise<WeatherReading[]> => {
        const response = await apiClient.get('/weather/recent', {
            params: {
                hours: hours
            }
        });
        return response.data;
    },

    getHistoricalWeather: async (startDateTime: string, endDateTime: string): Promise<WeatherReading[]> => {
        const response = await apiClient.get('/weather/historical', {
            params: {
                start_datetime: startDateTime,
                end_datetime: endDateTime
            }
        });
        return response.data;
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


