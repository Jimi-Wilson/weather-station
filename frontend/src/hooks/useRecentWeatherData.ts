import {useCallback, useEffect, useState} from "react";
import {weatherService} from "@/services/weatherService.ts";
import type {WeatherReading} from "@/services/types.ts";
import axios from "axios";

export const useRecentWeatherData = (hours: string) => {
    const [data, setData] = useState<WeatherReading[] | null>(null);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState<string | null>(null);

    const fetchData = useCallback(
        async () => {
            try {
                setError(null);
                const weatherData = await weatherService.getRecentWeather(hours);
                setData(weatherData);
            } catch (err) {
                if (axios.isAxiosError(err)) {
                    const errorMessage = err.response?.data?.detail || err.message || 'Failed to fetch recent weather data';
                    setError(errorMessage);
                } else {
                    setError('An unexpected error occurred');
                }
            } finally {
                setIsLoading(false);
            }
        }, [hours]);

    useEffect(() => {
        fetchData();
    }, [fetchData]);

    return {data, isLoading, error, fetchData};
};
