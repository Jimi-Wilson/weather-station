import {useCallback, useEffect, useState} from "react";
import {weatherService} from "@/services/weatherService.ts";
import type {WeatherReading} from "@/services/types.ts";
import axios from "axios";

export const useLatestWeatherData = (pollInterval: number = 30000) => {
    const [data, setData] = useState<WeatherReading | null>(null);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState<string | null>(null);

    const fetchData = useCallback(
        async () => {
            try {
                if (!data) setIsLoading(true);
                setError(null);
                const weatherData = await weatherService.getLatestWeather();
                setData(weatherData);
            } catch (err) {
                if (axios.isAxiosError(err)) {
                    const errorMessage = err.response?.data?.detail || err.message || 'Failed to fetch latest weather data';
                    setError(errorMessage);
                } else {
                    setError('An unexpected error occurred');
                }
            } finally {
                setIsLoading(false);
            }
        }, []);

    useEffect(() => {
        fetchData();
        const interval = setInterval(fetchData, pollInterval);
        return () => clearInterval(interval);
    }, [fetchData, pollInterval]);

    return {data, isLoading, error, fetchData};

}