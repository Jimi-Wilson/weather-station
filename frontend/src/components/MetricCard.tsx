import React from "react";
import {
    Card,
    CardContent,
    CardHeader,
    CardTitle,
} from "@/components/ui/card"

type MetricCardProps = {
    label: string;
    value: number;
    unit: string;
    icon: React.ReactNode;
    change: number | null;
}

const cardStyles = {
    'temperature': {
        color: 'text-orange-500',
    },
    'humidity': {
        color: 'text-blue-500',
    },
    'pressure': {
        color: 'text-purple-500',
    }
}

type CardStyleKey = keyof typeof cardStyles;

const MetricCard = ({ label, value, unit, icon, change}: MetricCardProps) => {
    const cardStyle = cardStyles[label.toLowerCase() as CardStyleKey];


    return (
        <Card>
            <CardHeader>
                <CardTitle>
                    <div className="flex items-center gap-2">
                        <div className={cardStyle.color}>
                            {icon}
                        </div>
                        <p>
                            {label}
                        </p>
                    </div>
                </CardTitle>
            </CardHeader>

            <CardContent>
                <p>{value} {unit}</p>
                {change !== null && (
                    <p className={`text-sm font-medium ${change > 0 ? 'text-green-500' : 'text-red-500'}`}>
                        {change > 0 ? '+' : ''}{change.toFixed(2)} since last update
                    </p>
                )}            </CardContent>
        </Card>

    );


}


export default MetricCard;